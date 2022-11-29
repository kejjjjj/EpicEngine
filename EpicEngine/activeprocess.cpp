#include "EpicEngine.h"

//purpose:
//keep track of if the opened process is open
void ActiveProcess::MonitorActiveProcess()
{
	if (!CurrentProcess.procdata.handle)
		return;

	//window closed
	if (!CurrentProcess.ProcessRunning()) {
		CurrentProcess.OnProcessKilled();
	}

}

bool ActiveProcess::ProcessRunning() const
{
	if (!procdata.handle)
		return false;

	DWORD exitcode;
	GetExitCodeProcess(procdata.handle, &exitcode);

	return exitcode == STILL_ACTIVE;
}
void ActiveProcess::OnProcessKilled()
{
	if (procdata.handle) {
		try {
			CloseHandle(procdata.handle);
		}
		catch (std::exception& ex) {
			std::cout << "Exception caught: " << ex.what() << '\n';
		}
	}
	procdata.handle = nullptr;

	std::cout << "OnProcessKilled(): STILL_ACTIVE != TRUE\n";
}
bool ActiveProcess::FetchModules()
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	DWORD i;

	module_u mod;

	CurrentProcess.modules.clear();

	if (!procdata.handle)
		return false;

	if (K32EnumProcessModules(procdata.handle, hMods, sizeof(hMods), &cbNeeded)) {
		const DWORD count = cbNeeded / sizeof(HMODULE);

		for (i = 0; i < count; i++) {
			char name[MAX_PATH];

			if (K32GetModuleBaseNameA(procdata.handle, hMods[i], name, sizeof(name) / sizeof(char))) {

				if (name[0] != '\0') {

					mod.name = name;
					if(!K32GetModuleInformation(procdata.handle, hMods[i], &mod.module, sizeof(MODULEINFO)))
						continue;

					modules.push_back(mod);
				}
				continue;
			}
			else {
				std::cout << "Error: GetModuleFileNameA(): " << fs::_GetLastError() << '\n';

			}

		}

	}
	else {
		std::cout << "Error: K32EnumProcessModules(): " << fs::_GetLastError() << '\n';

	}

	return !modules.empty();

}
void a(int b)
{
	std::cout << b;
	return;
}


int main()
{
}