#pragma once
#ifndef appwin
#define appwin
#include "EpicEngine.h"


class AppWindow  
{
public:

	void CornerActionButtons();

	ImVec2 Size;
	ImVec2 Pos;
	bool open;


private:
};

#endif