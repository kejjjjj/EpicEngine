#include "EpicEngine.h"

void Memoryview_t::Render()
{

	if (!MemoryView.window.open)
		return;

	ImGui::Begin("Memory view", 0, ImGuiWindowFlags_NoCollapse);
	ImGui::SetWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);

	MemoryView.OnRenderMemoryMap();

	MemoryView.window.Pos = ImGui::GetWindowPos();
	MemoryView.window.Size = ImGui::GetWindowSize();

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

	current_region_handle = start_address;
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

	const ImVec2 fp = ImGui::GetStyle().FramePadding;

	std::string addr = std::format("Address: {:x} | {}", (DWORD)start_address, process->name);
	ImGui::TextCentered(addr.c_str());

	const float itemHeight = ImGui::GetItemRectSize().y;

	ImGui::BeginGroup();

	ImGui::Dummy(ImVec2(0, 1)); //vertical alignment
	for (int i = 0; i < columns_visible; i++) {
		ImGui::Text("+0x%X", /*process->name.c_str(),*/ (DWORD)(((DWORD)start_address + (i * bytes_per_line)) - (DWORD)current_region_handle)); //module base offset
	}

	ImGui::EndGroup();
	ImGui::SameLine();

	ImGui::BeginChild("memmap", ImVec2(window.Size.x - 16, window.Size.y - fp.y - itemHeight*4), true);

	OnRenderMemoryMapContents();

	ImGui::EndChild();

	ImGuiIO* io = &ImGui::GetIO();

	if (io->MouseWheel < 0) {
		start_address = (void*)((DWORD)start_address + bytes_per_line);
	}
	else if (io->MouseWheel > 0) {
		start_address = (void*)((DWORD)start_address - bytes_per_line);
	}



}
void Memoryview_t::OnInvalidateData()
{
	start_address = nullptr;

}
void Memoryview_t::OnRenderMemoryMapContents() {
	if (!start_address) {
		ImGui::Text("No Process loaded!");
		return;
	}
	std::string fmt;
	USHORT offset_horizontal{0}, offset_vertical{0};
	bool address_invalid;
	
	for (int j = 0; j < bytes_per_line; j++) {

		ImGui::BeginGroup();
		for (int i = 0; i < columns_visible; i++) {

			//iterate all vertical offsets first to align the bytes properly

			const BYTE byte = CurrentProcess.read<BYTE>((DWORD)start_address + offset_vertical + offset_horizontal, address_invalid);

			if (address_invalid)
				fmt = "??";
			else {
				fmt = std::format("{:X}", byte);
				if (fmt.size() == 1)
					fmt.push_back('0');
			}
			ImGui::Text("%s ", fmt.c_str());


			offset_vertical += columns_visible;

		}
		ImGui::EndGroup();
		ImGui::SameLine();
		++offset_horizontal;
		offset_vertical = NULL;


	}

}