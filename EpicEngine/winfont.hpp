#pragma once

#ifndef winfont
#define winfont

#include "EpicEngine.h"

inline std::string root_directory;

enum FontEnum
{
	SegoeUI_LightSM,
	SegoeUI_LightM,
	SegoeUI_LightL,
	SegoeUI_LightXL,

	SegoeUI_SemiBoldSM,
	SegoeUI_SemiBoldM,
	SegoeUI_SemiBoldL,
	SegoeUI_SemiBoldXL


};

inline struct Font_s
{

	Font_s() : fonts(NULL) {};

	~Font_s(){ fonts.clear();}

	bool InsertFont(const char* name, std::string fontpath, float scale);
	ImFont* FetchFont(const char* name);

	std::vector< std::pair<ImFont*, const char*> > fonts;

	void LoadHardcodedFonts();

}Font;


#endif