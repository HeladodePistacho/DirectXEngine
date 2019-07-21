#include "ImGuiManager.h"

ImGuiManager::ImGuiManager()
{
		//Imgui Start
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.Fonts->AddFontDefault();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
