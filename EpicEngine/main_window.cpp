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
	//ImGui::NewLine();

	//static ImGuiStyle* s = &ImGui::GetStyle();
	
	if(ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			TabBarAction("Select Process\t", []() { ProcList::ProcWindow.OnCreateWindow(); });
			TabBarAction("Quit\t",			 []() { hWnd_main.window.open = false; });

			ImGui::EndMenu();
		}

	}ImGui::EndMenuBar();
	
}