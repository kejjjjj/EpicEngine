#include "EpicEngine.h"

void AppWindow::CornerActionButtons()
{
	ImVec2 bmins = ImVec2(Pos.x + Size.x - 50, Pos.y + 1);
	ImVec2 bmaxs = ImVec2(Pos.x + Size.x - 1, Pos.y + ImGui::GetStyle().FramePadding.y * 4);
	float a = 0;

	if (ImGui::IsHovered(bmins, bmaxs)) {
		a = 255;

	}

	ImVec2 midpoint = ImVec2(bmins.x + (bmaxs.x - bmins.x)/3, bmins.y-3);

	ImGui::PushFont(Font.fonts[SegoeUI_LightM].first);
	ImGui::GetForegroundDrawList()->AddRectFilled(bmins, bmaxs, IM_COL32(255, 0, 0, a));
	ImGui::GetForegroundDrawList()->AddText(midpoint, IM_COL32(255, 255, 255, 255), "X");
	ImGui::PopFont();

	if (ImGui::IsClicked(bmins, bmaxs)) {
		this->open = false;
	}
}