#include "EpicEngine.h"


void Scanning_t::Render()
{
	ImGui::BeginChild("scanning", ImVec2(hWnd_main.window.Size.x/2, 400), true);
	ImGui::TextCentered("work in progress..");
	ImGui::EndChild();
}