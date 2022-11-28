#pragma once

#ifndef mndwnd
#define mndwnd
#include "EpicEngine.h"


namespace MW 
{

	void Render();
	void RenderTabBar();
	void RenderedInMainWindow();
	void TabBarAction(const char* text, std::function<void()> a);
	void ButtonAction(const char* text, std::function<void()> a);

	inline std::vector<std::function<void()>> MainWndRendingFunc;
}

#endif