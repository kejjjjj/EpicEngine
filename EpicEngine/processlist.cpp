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

WProcess32* ProcessWindow::OnDrawProcess(WProcess32* i)
{

	static WProcess32* lastSelected = 0;
	static DWORD lastClicked = Sys_MilliSeconds();

	std::string text = std::format("{:X}-{}", i->ID, i->name);
	ImGui::Text("%s", text.c_str());

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();

	const ImVec2 bmins = ImVec2(min.x, min.y - ImGui::GetStyle().FramePadding.y / 2);
	const ImVec2 bmaxs = ImVec2(min.x + window.Size.x, max.y + ImGui::GetStyle().FramePadding.y / 2);


	if (i == lastSelected) {

		ImGui::GetWindowDrawList()->AddRectFilled(bmins, bmaxs, IM_COL32(50, 255, 255, 170));
	}

	if (ImGui::IsClicked(bmins, bmaxs) && ImGui::IsHovered(window.Pos, ImVec2(window.Pos.x + window.Size.x, window.Pos.y + 440))) {

		//ImGui::TextCentered("isClicked: %i", ImGui::IsClicked(bmins, bmaxs));

		lastSelected = i;

		if (Sys_MilliSeconds() - lastClicked < 200) {
			CurrentProcess = *i;

			OnKillWindow();
		}

		lastClicked = Sys_MilliSeconds();
		//return lastSelected;




	}
	return lastSelected;

}
void ProcessWindow::Render()
{
	ProcessWindow* PW = &ProcList::ProcWindow;
	if (!PW->window.open)
		return;


	ImGui::Begin("Select Process", 0, ImGuiWindowFlags_NoCollapse);


	static WProcess32* lastSelected;

	ImGui::BeginChild("cock", ImVec2(PW->window.Size.x, 400), true);


	for (auto& i : PW->Processes) {
		lastSelected = PW->OnDrawProcess(&i);
	}
	ImGui::EndChild();


	PW->window.Pos = ImGui::GetWindowPos();
	PW->window.Size = ImGui::GetWindowSize();
	
	if (ImGui::ButtonCentered("Open", 0.45f)) {
		CurrentProcess = lastSelected;
		PW->OnKillWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
		PW->window.open = false;

	PW->window.CornerActionButtons();

	ImGui::End();

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