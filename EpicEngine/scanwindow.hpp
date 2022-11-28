#pragma once

#ifndef __scanning
#define __scanning

#include "EpicEngine.h"

class Scanning_t
{
public:
	Scanning_t(){}
	~Scanning_t(){}

	static void Render();

private:
	DWORD results_found;

};

inline Scanning_t mem_scan;

#endif // ! scanning
