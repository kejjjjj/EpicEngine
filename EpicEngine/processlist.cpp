#include "EpicEngine.h"

DWORD ProcList::GetAvailableProcesses(std::vector<WProcess32>& Processes)
{
	DWORD procCount[1024];
	DWORD needed;
	if (!K32EnumProcesses(procCount, sizeof(procCount), &needed))
		return FALSE;

	BOOL count = needed / sizeof(DWORD);
	WProcess32 p;

	for (int i = 0; i < count; i++) {

		p.valid = false;

		if (!procCount[i])
			continue;

		p.handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procCount[i]);

		if (p.handle != NULL) {
			DWORD cbNeeded;

			if (K32EnumProcessModules(p.handle, &p.module, sizeof(HMODULE), &cbNeeded)) {

				char buff[124];

				K32GetModuleBaseNameA(p.handle, p.module, buff, 124);
				if (buff[0] != '\0')
					p.name = buff;

				//if(p.module)
				//	p.icon = ExtractIconA(p.module, buff, 0);

				
				
			}
			p.ID = GetProcessId(p.handle);

			CloseHandle(p.handle);
			p.valid = TRUE;

		}

		if (p.valid)
			Processes.push_back(p);
		

	}
	return Processes.size();

}

void ProcessWindow::OnDrawProcess(WProcess32* i)
{
	static WProcess32* lastSelected = i;
	static DWORD lastClicked = Sys_MilliSeconds();

	std::string text = std::format("{:X}-{}", i->ID, i->name);
	ImGui::BeginGroup();
	ImGui::PushItemWidth(300);
	ImGui::Text("%s.\t\t\t\t\t\t\t\t\t\t\t\t", text.c_str());
	ImGui::GetCurrentContext();
	
	ImGui::EndGroup();

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();

	if (i == lastSelected) {

		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(min.x, min.y - ImGui::GetStyle().FramePadding.y/2), ImVec2(max.x, max.y + ImGui::GetStyle().FramePadding.y/2), IM_COL32(50, 255, 255, 170));
	}

	if (ImGui::IsItemClicked()) {
		lastSelected = i;

		if (Sys_MilliSeconds() - lastClicked < 200) {
			CurrentProcess = *i;

			OnKillWindow();
		}

		lastClicked = Sys_MilliSeconds();





	}

}
void ProcessWindow::Render()
{
	ProcessWindow* PW = &ProcList::ProcWindow;
	if (!PW->window.open)
		return;


	ImGui::Begin("Select Process", 0, ImGuiWindowFlags_NoCollapse);

	for (auto& i : PW->Processes) {
		PW->OnDrawProcess(&i);
	}

	PW->window.Pos = ImGui::GetWindowPos();
	PW->window.Size = ImGui::GetWindowSize();
	
	PW->window.CornerActionButtons();

	ImGui::End();

	if (GetAsyncKeyState(VK_HOME) & 1) {
		printf("pos(%.1f, %.1f)\nsize(%.1f,%.1f)\n\n", PW->window.Pos.x, PW->window.Pos.y, PW->window.Size.x, PW->window.Size.y);
	}

}
void ProcessWindow::OnCreateWindow()
{
	ProcList::ProcWindow.Processes.clear();
	ProcList::ProcWindow.window.open = !ProcList::ProcWindow.window.open;
	ProcList::GetAvailableProcesses(ProcList::ProcWindow.Processes);

}
void ProcessWindow::OnKillWindow()
{
	Processes.clear();
	window.open = false;
}