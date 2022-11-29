#pragma once

#ifndef _membyte
#define _membyte

#include "EpicEngine.h"

#define MEMVIEW_REFRESHRATE 100 //updates every 100ms
#define MEMVIEW_X_PADDING 23.f

class Memoryview_t
{
public:
	Memoryview_t() : addrpopup(), current_region_handle(), start_address(), columns_visible(), bytes_per_line(), process(){}
	static void Render();
	void RenderTabBar();
	void OnWindowCreated();
	void OnWindowKilled();
	void OnRenderMemoryMap();
	void OnRenderMemoryMapContents();
	void OnGoToAddress();
	struct addresspopup {
		AppWindow window;

	}addrpopup;
	void OnInvalidateData();
	AppWindow window;
	WProcess32* process;

private:
	void* start_address; //first window offset starts here
	UPTR bytes_per_line;
	USHORT columns_visible;
	void* current_region_handle; //current module in the window
};

inline Memoryview_t MemoryView;
#endif