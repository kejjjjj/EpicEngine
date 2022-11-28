#pragma once

#ifndef _membyte
#define _membyte

#include "EpicEngine.h"

class Memoryview_t
{
public:
	static void Render();
	void OnWindowCreated();
	void OnWindowKilled();
	void OnRenderMemoryMap();
	void OnRenderMemoryMapContents();
	void OnInvalidateData();
	AppWindow window;
	WProcess32* process;

private:
	void* start_address; //first window offset starts here
	USHORT bytes_per_line;
	USHORT columns_visible;
	void* current_region_handle; //current module in the window
};

inline Memoryview_t MemoryView;
#endif