#pragma once
#ifndef appwin
#define appwin
#include "EpicEngine.h"


class AppWindow  
{
public:

	AppWindow() : Size(), Pos(), open(){}
	void CornerActionButtons();

	ImVec2 Size;
	ImVec2 Pos;
	bool active;
	bool open;

private:
};

#endif