#include "LightComponent.h"
#include "ImGui/imgui.h"

LightComponent::LightComponent(LIGHT_TYPE type) : light_type(type)
{
	light_color[0] = 1.0f;
	light_color[1] = 1.0f;
	light_color[2] = 1.0f;
}

void LightComponent::DrawUI()
{
	if (ImGui::TreeNode("Light Component"))
	{
		ImGui::Text("Light Type: ");
		if (ImGui::BeginCombo("##Light combo", LightTypeToChar()))
		{
			if (ImGui::Selectable("Directional Light"))
				light_type = LIGHT_TYPE::DIRECTIONAL_LIGHT;

			if (ImGui::Selectable("Point Light"))
				light_type = LIGHT_TYPE::POINT_LIGHT;

			ImGui::EndCombo();
		}

		ImGui::Separator();

		ImGui::Text("Light Color: ");

		ImGui::PushItemWidth(120.0f);
		ImGui::ColorPicker3("##Light color picker", light_color, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_::ImGuiColorEditFlags_NoSidePreview);
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}

const char * LightComponent::LightTypeToChar()
{
	switch (light_type)
	{
	case DIRECTIONAL_LIGHT:
		return "Directional Light";
		break;
	case POINT_LIGHT:
		return "Point Light";
		break;
	}
}
