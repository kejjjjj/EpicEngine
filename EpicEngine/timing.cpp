#include "EpicEngine.h"


DWORD Sys_MilliSeconds()
{
	return timeGetTime() - sys_baseTime;
}