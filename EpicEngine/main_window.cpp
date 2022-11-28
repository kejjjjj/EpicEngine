#include "EpicEngine.h"

void MW::Render()
{
	ImGui::TextCentered("Selected Process: %s", CurrentProcess.procdata.handle != nullptr ? CurrentProcess.procdata.name.c_str() : "NONE");

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
void MW::ButtonAction(const char* text, std::function<void()> a)
{
	if (ImGui::Button(text))
		a();

	//ImVec2 min = ImGui::GetItemRectMin();
	//ImVec2 max = ImGui::GetItemRectMax();


	//if (ImGui::IsItemHovered()) {
	//	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(min.x, min.y - ImGui::GetStyle().FramePadding.y), ImVec2(max.x, max.y + ImGui::GetStyle().FramePadding.y), IM_COL32(50, 50, 50, 170));
	//	if (ImGui::IsItemClicked())
	//		a();
	//}
	//ImGui::Separator();
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

	MW::RenderedInMainWindow();
	
}
void MW::RenderedInMainWindow()
{

	for (auto& i : MainWndRendingFunc) { 
		i(); 
	}

	ButtonAction("Memory view", []() { MemoryView.OnWindowCreated(); });
		
}