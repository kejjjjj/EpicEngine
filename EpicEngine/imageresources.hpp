#pragma once

#ifndef imgres
#define imgres

#include "EpicEngine.h"

class Image_s
{
public:

	Image_s() : name(), path(), registered(FALSE), texture(0), width(0), height(0) {}
	~Image_s() {}

	bool Register(std::string const path);
	bool CreateD3DX11Texture(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	bool CreateD3DX11Texture(std::string refName = "");

	std::string name;
	ID3D11ShaderResourceView* texture;
	int width, height;
private:
	std::string path;
	bool registered;

	//std::vector<std::pair<std::string, ID3D11Texture2D>> images;

};

class Resource_s : public Font_s
{
public:
	Resource_s() : images() {}


	Image_s* GetImageByName(const char* name);
	void AddTexture(std::string const _path, std::string refName = "");
	std::vector<Image_s> images;
	Font_s Font;
	void LoadAllResources();
};

inline Resource_s rsc;

#endif