#pragma once

#ifndef actproce
#define actproce

#include "EpicEngine.h"


class ActiveProcess
{
public:
	ActiveProcess(){}
	ActiveProcess(WProcess32 proc) {
		data = proc;
	}
	ActiveProcess(WProcess32* proc) {
		memcpy_s(&data, sizeof(WProcess32), proc, sizeof(WProcess32));
	}
	void Initialize(WProcess32 proc) {
		data = proc;
	}
	void Initialize(WProcess32* proc) {
		memcpy_s(&data, sizeof(WProcess32), proc, sizeof(WProcess32));
	}

	WProcess32 data;


private:
};

inline ActiveProcess CurrentProcess;

#endif