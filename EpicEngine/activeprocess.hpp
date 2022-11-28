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
	t read(const DWORD v, bool& failed) {

		//object should have PROCESS_VM_READ flag
		t end;
		MEMORY_BASIC_INFORMATION meminfo;
		static DWORD last_error_happened = 0;

		failed = true;

		memory.last_read_block = reinterpret_cast<void*>(v);
		const SIZE_T ret = VirtualQueryEx(procdata.handle, (LPCVOID)v, &meminfo, sizeof(MEMORY_BASIC_INFORMATION));

		if (ret == ERROR_INVALID_PARAMETER || !ret) {
			if (Sys_MilliSeconds() > last_error_happened + 1000) {
				std::cout << "VirtualQueryEx(): " << fs::_GetLastError() << '\n';
				last_error_happened = Sys_MilliSeconds();
			}

			return 0;
		}
		else if (meminfo.Protect == 1) {
			return 0;
			
		}

		if (ReadProcessMemory(this->procdata.handle, (LPCVOID)v, &end, sizeof(t), &memory.lpNumberOfBytesRead) == FALSE) {

			if (Sys_MilliSeconds() > last_error_happened + 1000) {
				std::cout << "ReadProcessMemory(): " << fs::_GetLastError() << '\n'
					<< "p.name  : " << procdata.name << '\n'
					<< "p.handle: " << procdata.handle << '\n';

				last_error_happened = Sys_MilliSeconds();
			}
			return 0;
		}

		failed = false;
		return end;
	}


private:
};

inline ActiveProcess CurrentProcess;

#endif