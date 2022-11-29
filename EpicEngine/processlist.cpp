#include "EpicEngine.h"

DWORD ProcList::GetAvailableProcesses(std::vector<WProcess32>& Processes)
{
	ProcList::ProcWindow.OnCloseAllHandles(0);

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

				if (!p.module)
					std::cout << "INVALID HMODULE\n";

				//K32GetProcessImageFileNameA(p.handle, buff, 124);
				if (!K32GetModuleBaseNameA(p.handle, p.module, buff, 124)) {
					std::cout << "K32GetModuleBaseNameA: " << fs::_GetLastError() << '\n';
				}

				if (buff[0] != '\0') {
					size_t len = strlen(buff)+1;
					for (int i = 0; i < len; i++) {

						p.name.push_back(tolower(buff[i]));
						//p.name = buff;
					}
				}
				else
					p.name = "Unable to get name";

				//if(p.module)
				//	p.icon = ExtractIconA(p.module, buff, 0);

				
				
			}
			else {
				CloseHandle(p.handle);
				continue;
				//std::cout << "K32EnumProcessModules(p.handle, &p.module, sizeof(HMODULE), &cbNeeded): " << fs::_GetLastError() << '\n';
			}
			p.ID = GetProcessId(p.handle);

		//	std::cout << p.name << " handle is: " << std::dec << p.handle << '\n';

			//CloseHandle(p.handle);
			p.valid = TRUE;

		}/*else
			std::cout << "INVALID handle! GetLastError(): \n" << fs::_GetLastError().c_str();*/

		if (p.valid)
			Processes.push_back(p);
		
		p.name.clear();

	}
	return Processes.size();

}

WProcess32* ProcessWindow::OnDrawProcess(WProcess32* i, const float height)
{
	float initial_y = window.Pos.y + 45;
	static WProcess32* lastSelected = 0;
	static UPTR lastClicked = Sys_MilliSeconds();

	const std::string text = std::format("{}-{:x}-{}", i->ID, (UPTR)i->module, i->name);
	ImGui::Text("%s", text.c_str());

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();

	const ImVec2 bmins = ImVec2(min.x, min.y - ImGui::GetStyle().FramePadding.y / 2);
	const ImVec2 bmaxs = ImVec2(min.x + window.Size.x, max.y + ImGui::GetStyle().FramePadding.y / 2);


	if (i == lastSelected) {

		ImGui::GetWindowDrawList()->AddRectFilled(bmins, bmaxs, IM_COL32(50, 255, 255, 170));
	}

	if (ImGui::IsClicked(bmins, bmaxs) && ImGui::IsHovered(ImVec2(window.Pos.x, initial_y), ImVec2(window.Pos.x + window.Size.x, initial_y + height))) {

		//ImGui::TextCentered("isClicked: %i", ImGui::IsClicked(bmins, bmaxs));

		lastSelected = i;

		if (Sys_MilliSeconds() - lastClicked < 200) {
			OnProcessSelected(*i);
			//CurrentProcess = *i;
			//MemoryView.process = &CurrentProcess.procdata;
			//OnCloseAllHandles(i);
			//OnKillWindow();
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

	static std::string search;

	ImGui::InputText("Search", &search);

	const float height = PW->window.Size.y - 200;
	ImGui::BeginChild("yepp", ImVec2(PW->window.Size.x, height), true);
	ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	

	for (auto& i : PW->Processes) {
		if(i.name.find(search) != std::string::npos || search.empty())
			lastSelected = PW->OnDrawProcess(&i, height);
	}
	ImGui::EndChild();


	PW->window.Pos = ImGui::GetWindowPos();
	PW->window.Size = ImGui::GetWindowSize();
	
	if (ImGui::ButtonCentered("Open", 0.45f)) {
		PW->OnProcessSelected(*lastSelected);
		//CurrentProcess = lastSelected;
		//PW->OnCloseAllHandles(lastSelected);
		//PW->OnKillWindow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		PW->OnCloseAllHandles(0);
		PW->OnKillWindow();

	}

	PW->window.CornerActionButtons();

	ImGui::End();

}
void ProcessWindow::OnCloseAllHandles(WProcess32* exception)
{
	if (this->Processes.empty())
		return;

	for (auto& j : this->Processes) {
		if (!j.handle)
			continue;

		if (exception) {
			if (exception->handle != j.handle) {
				std::cout << "closing handle for [" << j.name << "]\n";
				CloseHandle(j.handle);
				j.handle = 0;
			}
		}
		else {
			std::cout << "closing handle for [" << j.name << "]\n";
			CloseHandle(j.handle);
			j.handle = 0;
		}

	}
}
void ProcessWindow::OnProcessSelected(WProcess32 process)
{
	MemoryView.OnInvalidateData();
	CurrentProcess = process;
	MemoryView.process = &CurrentProcess.procdata;
	OnCloseAllHandles(&process);
	OnKillWindow();
	CurrentProcess.FetchModules();
}
void ProcessWindow::OnCreateWindow()
{
	ProcList::ProcWindow.Processes.clear();
	ProcList::ProcWindow.window.open = !ProcList::ProcWindow.window.open;
	ProcList::GetAvailableProcesses(ProcList::ProcWindow.Processes);

}
void ProcessWindow::OnKillWindow()
{
	//OnCloseAllHandles(0);
	Processes.clear();
	window.open = false;
}