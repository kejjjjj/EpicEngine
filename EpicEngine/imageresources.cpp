#include "EpicEngine.h"

bool Image_s::Register(std::string const _path)
{
	path = root_directory + "\\" + _path;


	std::string _name = fs::F_GetFileName(_path);
	name = fs::removeFileExtension(_name, 4);
	registered = TRUE;

}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Image_s::CreateD3DX11Texture(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL) {
		FatalError("Image_s::CreateD3DX11Texture() failed with: %s\nPossibly passed the wrong filename?", stbi_failure_reason());
		return false;
	}
	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	hWnd_main.g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hWnd_main.g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}
bool Image_s::CreateD3DX11Texture(std::string refName)
{
	if (!refName.empty())
		name = refName;


	if (!registered) {
		FatalError("Image_s::CreateD3DX11Texture() called without the object being initialized first!");
		return FALSE;
	}

	//const HRESULT hr = D3DX11CreateTextureFromFileA(hWnd_main.g_pd3dDevice, path.c_str(), NULL, NULL, &texture, NULL);

	bool hr = CreateD3DX11Texture(path.c_str(), &texture, &width, &height);

	if (!hr) {
		FatalError("Image_s::CreateD3DX11Texture(): Invalid image data\n");
	}

	return hr;
}
void Resource_s::AddTexture(std::string const _path, std::string refName)
{
	Image_s img;
	if (!img.Register(_path))
		return;

	if(!img.CreateD3DX11Texture(refName))
		return;

	this->images.push_back(img);
}
Image_s* Resource_s::GetImageByName(const char* name)
{
	for (auto& i : this->images) {
		if (!i.name.compare(name))
			return &i; 
	}

	return nullptr;
}
void Resource_s::LoadAllResources()
{
	Font.LoadHardcodedFonts();

	AddTexture("images\\close32.png", "close32");
	AddTexture("images\\minimize32.png", "minimize32");
	AddTexture("images\\maximize32.png", "maximize32");


}