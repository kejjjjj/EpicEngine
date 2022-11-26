#pragma once

#ifndef mndwnd
#define mndwnd
#include "EpicEngine.h"


namespace MW 
{

	void Render();
	void RenderTabBar();
	void TabBarAction(const char* text, std::function<void()> a);
}

#endif