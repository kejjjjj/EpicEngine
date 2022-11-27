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