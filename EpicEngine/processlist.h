#pragma once

#ifndef proclist
#define proclist

#include "EpicEngine.h"

struct WProcess32
{

	HANDLE handle;
	HMODULE module;
	UPTR ID;
	std::string name;
	HICON icon; //idk how to use this with directx
	bool valid;

};
class ProcessWindow
{
public:

	ProcessWindow() : window(){}
	~ProcessWindow() { Processes.clear(); }

	static void Render();
	WProcess32* OnDrawProcess(WProcess32* prc, const float height);
	void OnKillWindow();
	void OnCreateWindow();
	void OnCloseAllHandles(WProcess32* exception);
	void OnProcessSelected(WProcess32 process);
	
	AppWindow window;

	std::vector<WProcess32> Processes;

private:
};

namespace ProcList
{
	DWORD GetAvailableProcesses(std::vector<WProcess32>& Processes);
	inline ProcessWindow ProcWindow;
}

#endif // !proclist
