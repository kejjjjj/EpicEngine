#include "EpicEngine.h"
UPTR AddressParser::Parse()
{
	if (input.empty()) {
		MessageBoxA(NULL, "Error", "Empty string", MB_ICONERROR);
		return 0;
	}

	const auto TokenizeString = [](std::string str, std::vector<token_t>& tokens, char d1 = '\0', char d2 = '\0', char d3 = '\0', char d4 = '\0', char d5 = '\0') -> SIZE_T {
		
		//std::stringstream ss(str);
		std::string token;

		for (auto& i : str) {
		//	std::cout << "i: " << i << '\n';
			if (i != d1 && i != d2 && i != d3 && i != d4 && i != d5)  
				token.push_back(i);
			else {
				
				//token.push_back(token_t{token, i});
				if (i != d5) {
					tokens.push_back(token_t{ token, i });
					token.clear();
				}
			}
		}
		tokens.push_back({ token, '\0'});

		return tokens.size();
	};

	const auto TokenError = [](token_t* token) -> void {
		char buff[MAX_PATH];
		sprintf_s(buff, "EvaluateToken(): Invalid token passed: '%s'", token->content.c_str());
		MessageBoxA(NULL, buff, "Error", MB_ICONERROR);
	};

	std::vector<token_t> tokens;

	SIZE_T token_count = TokenizeString(input, tokens, '+', '-', '*', '/', ' ');

	UPTR result(0);



	for (size_t count = 0; count < token_count; count++) {
		token_t token = tokens[count];

		if (!EvaluateToken(&token)) {
			TokenError(&token);
			return 0;
		}

		if (token._operator != '\0' && count < token_count-1) { //has operator and is not the last token
			token_t next_token = tokens[count + 1];

			if (!EvaluateToken(&next_token)) {
				TokenError(&next_token);

			}

			result += EvaluateExpression(&token, &next_token, token._operator);

			std::cout << "token expression: [" << std::hex << token.value << ' ' << token._operator << ' ' << std::hex << next_token.value << "]: " 
				<< std::hex << EvaluateExpression(&token, &next_token, token._operator) << '\n';

			count++;
		}
		else if(token_count > 1) {
			token_t previous_token = tokens[count - 1];

			result += EvaluateExpression(&previous_token, &token, previous_token._operator);
		}
		else
			result += token.value;



	}

	std::cout << "end result: " << std::hex << result << '\n';
	MemoryView.start_address = (void*)result;

	return 1;

}
UPTR AddressParser::EvaluateModule(token_t* token)
{

	for (auto& i : this->module) {
		if (!token->content.compare(i.name))
			return (UPTR)i.module.lpBaseOfDll;
	}
	return 0;
}
bool AddressParser::EvaluateToken(token_t* token)
{
	token->value = EvaluateModule(token);
	if (token->value)
		return token->value;

	//token doesn't include a module, so make sure the token only has hexadecimal characters and error if it doesn't

	for (auto& i : token->content) {
		if ((i >= 'a' && i <= 'f') || (i >= 'A' && i <= 'F') || (i >= '0' && i <= '9')) //is hexadecimal if true
			continue;

		//char buff[MAX_PATH];
		//sprintf_s(buff, "EvaluateToken(): Invalid token passed: '%s'", token->content.c_str());
		//MessageBoxA(NULL, buff, "Error", MB_ICONERROR);
		return 0;

	}

	std::stringstream ss(token->content);

	ss >> std::hex >> token->value;

	return true;
}
UPTR AddressParser::EvaluateExpression(token_t* token, token_t* next_token, char _operator)
{
	switch (_operator) {
	case '+':
		return token->value + next_token->value;

	case '-':
		return token->value - next_token->value;

	case '*':
		return token->value * next_token->value;

	case '/':
		return token->value / next_token->value;

	default:
		return 0;
	}
}