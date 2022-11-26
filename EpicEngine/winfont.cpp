
#include "EpicEngine.h"

bool Font_s::InsertFont(const char* name, std::string fontpath, float scale)
{
	ImGuiIO* io = &ImGui::GetIO();

	if (!io) {
		FatalError("Font_s::InsertFont(): ImGui::GetIO() returned 0");
		return false;
	}
	fonts.push_back(std::make_pair(io->Fonts->AddFontFromFileTTF(fontpath.c_str(), scale), name));

	return true;
}
ImFont* Font_s::FetchFont(const char* name)
{
	for (const auto& i : fonts) {
		if (!strcmp(name, i.second))
			return i.first;
	}

	return nullptr;
}
void Font_s::LoadHardcodedFonts()
{
	InsertFont("SegoeUILightSM", (root_directory + "\\fonts\\segoeuil.ttf"), 20);
	InsertFont("SegoeUILightM", (root_directory + "\\fonts\\segoeuil.ttf"), 40);
	InsertFont("SegoeUILightL", (root_directory + "\\fonts\\segoeuil.ttf"), 50);
	InsertFont("SegoeUILightXL", (root_directory + "\\fonts\\segoeuil.ttf"), 60);

	InsertFont("SegoeUISemiboldSM", (root_directory + "\\fonts\\seguisb.ttf"), 20);
	InsertFont("SegoeUISemiboldM", (root_directory + "\\fonts\\seguisb.ttf"), 40);
	InsertFont("SegoeUISemiboldL", (root_directory + "\\fonts\\seguisb.ttf"), 50);
	InsertFont("SegoeUISemiboldXL", (root_directory + "\\fonts\\seguisb.ttf"), 60);


}