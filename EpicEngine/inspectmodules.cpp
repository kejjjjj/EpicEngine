#include "EpicEngine.h"


//called from Memoryview_t::RenderTabBar()
void ModuleWindow_t::Render()
{
	if (!window.open)
		return;


	ImGui::Begin("Inspect Modules", 0, ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_FirstUseEver);

	window.Pos = ImGui::GetWindowPos();
	window.Size = ImGui::GetWindowSize();

	window.active = ImGui::IsWindowFocused();

	std::string fmt;

	for (const auto& i : CurrentProcess.modules) {
		fmt = std::format("{} - {:X}/{:X}", i.name, (UPTR)i.module.lpBaseOfDll, (UPTR)i.module.lpBaseOfDll + (UPTR)i.module.SizeOfImage);
		ImGui::Text(fmt.c_str());
	}
	
	window.CornerActionButtons();

	ImGui::End();

}
void ModuleWindow_t::OnOpenModuleWindow()
{
	if (!CurrentProcess.ProcessRunning()) {
		MessageBoxA(NULL, "No process selected", "Error", MB_ICONERROR);
		return;
	}
	
	if (!CurrentProcess.FetchModules()) {
		MessageBoxA(NULL, "No modules could be found!", "Error", MB_ICONERROR);
		window.open = false;
		return;
	}
	window.open = true;

	

}