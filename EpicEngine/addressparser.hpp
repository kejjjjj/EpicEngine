#pragma once

#ifndef addrpars
#define addrpars

#include "EpicEngine.h"

class AddressParser
{
public:
	enum class PARSETYPE{
		ADDRESS,
		MODULENAME
	};

	AddressParser(const std::string string, std::vector<module_u>& mod) {
		input = string;
		module = mod;
	};
	AddressParser() : input(), module(){};

	void Initialize(const std::string string, std::vector<module_u>& mod) {
		input = string;
		module = mod;
	};

	UPTR Parse();
	

	struct token_t {
		token_t() : content(), _operator('\0'), value(0) {}

		token_t operator=(token_t a) {
			content = a.content;
			_operator = a._operator;
			value = a.value;
		}
		token_t(std::string token, char a_operator) {
			content = token;
			_operator = a_operator;
			value = 0;
		}

		std::string content;
		char _operator;
		UPTR value;
	};

private:
	bool EvaluateToken(token_t* token);
	UPTR EvaluateExpression(token_t* token, token_t* next_token, char _operator);
	UPTR EvaluateModule(token_t* token);

	std::string input;
	std::vector<module_u> module;
};

#endif