#include "EpicEngine.h"


bool Is32BitOS()
{
	SYSTEM_INFO a;
	GetNativeSystemInfo(&a);
	return a.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL;

}