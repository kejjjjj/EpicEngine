#pragma once

#ifndef actproce
#define actproce

#include "EpicEngine.h"



class ActiveProcess
{
public:
	

	ActiveProcess() : memory(), procdata() {}
	ActiveProcess(WProcess32 proc) : memory(), procdata() { procdata = proc; }
	ActiveProcess(WProcess32* proc) : memory(), procdata() { memcpy_s(&procdata, sizeof(WProcess32), proc, sizeof(WProcess32)); }

	void Initialize(WProcess32 proc) { procdata = proc; }
	void Initialize(WProcess32* proc) { memcpy_s(&procdata, sizeof(WProcess32), proc, sizeof(WProcess32));}

	bool ProcessRunning() const;
	void OnProcessKilled();
	static void MonitorActiveProcess();

	WProcess32 procdata;

	union memory_u {
		void* last_read_block;
		SIZE_T lpNumberOfBytesRead;
	}memory;

	template<typename t>
	t read(const DWORD v) {

		//object should have PROCESS_VM_READ flag
		float end;
		memory.last_read_block = reinterpret_cast<void*>(v);

		if (ReadProcessMemory(this->procdata.handle, (LPCVOID)v, &end, sizeof(t), &memory.lpNumberOfBytesRead) == FALSE) {
			std::cout << "ReadProcessMemory(): " << fs::_GetLastError() << '\n';
			return 0;
		}

		return end;
	}


private:
};

inline ActiveProcess CurrentProcess;

#endif