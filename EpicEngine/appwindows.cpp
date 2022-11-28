#include "EpicEngine.h"

void AppWindow::CornerActionButtons()
{
	ImVec2 bmins = ImVec2(Pos.x + Size.x - 50, Pos.y + 1);
	ImVec2 bmaxs = ImVec2(Pos.x + Size.x - 1, Pos.y + ImGui::GetStyle().FramePadding.y * 4);
	float a = 0;
	static ImTextureID close, icon;
	static Image_s* img;

	if (ImGui::IsHovered(bmins, bmaxs)) {
		a = 255;

	}

	ImVec2 midpoint = ImVec2(bmins.x + (bmaxs.x - bmins.x) / 3, bmins.y - 3);

	ImGui::PushFont(rsc.Font.fonts[SegoeUI_LightM].first);
	ImGui::GetForegroundDrawList()->AddRectFilled(bmins, bmaxs, IM_COL32(255, 0, 0, a));

	if (!close) {
		img = rsc.GetImageByName("close32");
		if (img)
			close = img->texture;
	}
	else
		ImGui::GetForegroundDrawList()->AddImage(close, ImVec2(bmins.x + 14, bmins.y + 8), ImVec2(bmaxs.x - 14, bmaxs.y - 8));
	
	if (!icon) {
		img = rsc.GetImageByName("icon128");
		if (img)
			icon = img->texture;
	}
	else
		ImGui::GetForegroundDrawList()->AddImage(icon, ImVec2(Pos.x + 10, Pos.y + 5), ImVec2(Pos.x + 42, Pos.y + 37));


	ImGui::PopFont();

	if (ImGui::IsClicked(bmins, bmaxs)) {
		this->open = false;
	}
}