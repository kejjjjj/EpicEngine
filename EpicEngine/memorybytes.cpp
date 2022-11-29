#include "EpicEngine.h"

void Memoryview_t::RenderTabBar()
{
	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("Tools")) {

			//TabBarAction("Select Process\t", []() { ProcList::ProcWindow.OnCreateWindow(); });
			MW::TabBarAction("Inspect Modules\t", []() { moduleWnd.OnOpenModuleWindow(); });

			ImGui::EndMenu();
		}

	}ImGui::EndMenuBar();


	moduleWnd.Render();

}
void Memoryview_t::Render()
{

	if (!MemoryView.window.open)
		return;

	ImGui::Begin("Memory view", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);

	MemoryView.RenderTabBar();

	MemoryView.OnRenderMemoryMap();

	MemoryView.window.Pos = ImGui::GetWindowPos();
	MemoryView.window.Size = ImGui::GetWindowSize();

	MemoryView.window.active = ImGui::IsWindowFocused();


	MemoryView.window.CornerActionButtons();

	ImGui::End();

}
void Memoryview_t::OnWindowCreated()
{
	process = &CurrentProcess.procdata;
	bytes_per_line = 24;
	columns_visible = 16;
	if(!start_address)
		start_address = process->handle != nullptr ? process->module : 0;

	current_region_handle = CurrentProcess.FetchModuleFromAddress((UPTR)start_address);

	this->window.open = true;

}
void Memoryview_t::OnWindowKilled()
{
	this->window.open = false;
}
void Memoryview_t::OnRenderMemoryMap()
{
	if(!start_address && process->module)
		start_address = process->module;

	OnGoToAddress();

	const ImVec2 fp = ImGui::GetStyle().FramePadding;

	std::string addr = std::format("Address: {:x} | {}", (UPTR)start_address, current_region_handle != nullptr ?  current_region_handle->name : "N/A");
	ImGui::TextCentered(addr.c_str());

	const float itemHeight = ImGui::GetItemRectSize().y;

	ImGui::BeginGroup();

	ImGui::Dummy(ImVec2(0, 1)); //vertical alignment

	for (UPTR i = 0; i < columns_visible; i++) {
		addr = std::format("0x{:X}", ((UPTR)start_address + (UPTR)(i * bytes_per_line)));
		ImGui::Text(addr.c_str());
	}

	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginChild("memmap", ImVec2(window.Size.x - 16, window.Size.y - fp.y - itemHeight * 4), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

	OnRenderMemoryMapContents();

	ImGui::EndChild();

	ImGuiIO* io = &ImGui::GetIO();

	if (window.active) {
		if (io->MouseWheel < 0) {
			start_address = (void*)((UPTR)start_address + bytes_per_line);
			current_region_handle = CurrentProcess.FetchModuleFromAddress((UPTR)start_address);

		}
		else if (io->MouseWheel > 0) {
			start_address = (void*)((UPTR)start_address - bytes_per_line);
			current_region_handle = CurrentProcess.FetchModuleFromAddress((UPTR)start_address);

		}
	}



}
void Memoryview_t::OnInvalidateData()
{
	start_address = nullptr;

}
void Memoryview_t::OnRenderMemoryMapContents() {
	if (!CurrentProcess.ProcessRunning()) {
		ImGui::Text("No Process loaded!");
		return;
	}
	std::string fmt;
	USHORT offset_horizontal{0}, offset_vertical{0};
	bool address_invalid;
	
	//static DWORD time = Sys_MilliSeconds();
	//if (time + MEMVIEW_REFRESHRATE < Sys_MilliSeconds()) {
	//	time = Sys_MilliSeconds();
	//	return;
	//}

	for (int j = 0; j < bytes_per_line; j++) {

		ImGui::BeginGroup();

		for (int i = 0; i < columns_visible; i++) {

			//iterate all vertical offsets first to align the bytes properly

			const BYTE byte = CurrentProcess.read<BYTE>((UPTR)start_address + offset_vertical + offset_horizontal, address_invalid);

			if (address_invalid)
				fmt = "? ?";
			else {
				fmt = std::format("{:X}", byte);
				if (fmt.size() == 1)
					fmt.push_back('0');
			}
			ImGui::Text("%s ", fmt.c_str());
			ImGui::SameLine();
			ImGui::Dummy(ImVec2((MEMVIEW_X_PADDING - ImGui::GetItemRectSize().x) - 10, 0));
			if (GetAsyncKeyState(VK_HOME) & 1) {
				std::cout << "width off '" << std::hex << (ULONG)byte << "': " << ImGui::GetItemRectSize().x << '\n';
			}

			offset_vertical += columns_visible;

		}
		ImGui::EndGroup();
		ImGui::SameLine();

		++offset_horizontal;
		offset_vertical = NULL;


	}

}
void Memoryview_t::OnGoToAddress()
{
	static ImGuiIO* io = &ImGui::GetIO();

	if (io->KeyCtrl && io->KeysDown['G']) {
		std::cout << "CTRL + G\n";
		addrpopup.window.open = true;
	}

	if (!addrpopup.window.open)
		return;

	static std::string addr;


	ImGui::Begin("Go To Address", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::TextCentered("where?");

	ImGui::InputText("##01", &addr);

	if (ImGui::ButtonCentered("OK", 0.2f, ImVec2(100, 0)) || io->KeysDown[ImGuiKey_Enter]) {
		if (addr.empty()) {
			MessageBoxA(NULL, "Invalid Address", "Error", MB_ICONERROR);
			return;
		}
		AddressParser parser(addr, CurrentProcess.modules);
		parser.Parse();
		//std::stringstream ss(addr);

		//UPTR UPTR_addr;
		//ss >> std::hex >> UPTR_addr;

		//start_address = (void*)UPTR_addr;
		//addrpopup.window.open = false;


	}ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(100, 0))) {
		addrpopup.window.open = false;
	}

	addrpopup.window.CornerActionButtons();

	addrpopup.window.Pos = ImGui::GetWindowPos();
	addrpopup.window.Size = ImGui::GetWindowSize();

	ImGui::End();

}
void Memoryview_t::OnSetActiveAddress(void* point)
{
	current_region_handle = CurrentProcess.FetchModuleFromAddress((UPTR)point);

	if (!current_region_handle)
		return;

	start_address = point;


}