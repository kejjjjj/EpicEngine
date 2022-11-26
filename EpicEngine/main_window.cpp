#include "EpicEngine.h"

void MW::Render()
{
	ImGui::TextCentered("Selected Process: %s", CurrentProcess.data.handle != nullptr ? CurrentProcess.data.name.c_str() : "NONE");

}

void MW::TabBarAction(const char* text, std::function<void()> a)
{
	ImGui::Text(text);

	ImVec2 min = ImGui::GetItemRectMin();
	ImVec2 max = ImGui::GetItemRectMax();


	if (ImGui::IsItemHovered()) {
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(min.x, min.y-ImGui::GetStyle().FramePadding.y), ImVec2(max.x, max.y+ImGui::GetStyle().FramePadding.y), IM_COL32(50, 50, 50, 170));
		if (ImGui::IsItemClicked())
			a();
	}
	ImGui::Separator();
}

void MW::RenderTabBar()
{
	ImGui::NewLine();

	ImGuiStyle* s = &ImGui::GetStyle();
	
	if(ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			TabBarAction("Select Process\t", []() { ProcList::ProcWindow.OnCreateWindow(); });
			TabBarAction("Quit\t",			 []() { hWnd_main.window.open = false; });

			ImGui::EndMenu();
		}

	}ImGui::EndMenuBar();
	
}
void AppWindow::CornerActionButtons()
{
	ImVec2 bmins = ImVec2(Pos.x + Size.x - 50, Pos.y + 1);
	ImVec2 bmaxs = ImVec2(Pos.x + Size.x - 1, Pos.y + ImGui::GetStyle().FramePadding.y * 4);
	float col = 0;

	if (ImGui::IsHovered(bmins, bmaxs)) {
		col = 255;
	
	}

	ImVec2 midpoint = ImVec2(bmins.x + (bmaxs.x - bmins.x), bmins.y + (bmaxs.y - bmins.y));

	ImGui::PushFont(Font.fonts[SegoeUI_SemiBoldM].first);
	ImGui::GetForegroundDrawList()->AddRectFilled(bmins, bmaxs, IM_COL32(col, 0, 0, 255));
	ImGui::GetForegroundDrawList()->AddText(ImVec2(bmins.x + 15, bmins.y-2), IM_COL32(255, 255, 255, 255), "X");
	ImGui::PopFont();
	if (ImGui::IsClicked(bmins, bmaxs)) {
		this->open = false;
	}
}