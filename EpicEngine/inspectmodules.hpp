#pragma once

#ifndef insmod
#define insmod

#include "EpicEngine.h"

class ModuleWindow_t
{
public:
	void Render();
	void OnOpenModuleWindow();

	AppWindow window;

};

inline ModuleWindow_t moduleWnd;
#endif