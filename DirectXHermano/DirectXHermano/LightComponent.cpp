#include "LightComponent.h"
#include "ImGui/imgui.h"

LightComponent::LightComponent(LIGHT_TYPE type) : light_type(type)
{
	buffer_struct = {};
	buffer_struct.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer_struct.position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	buffer_struct.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

	buffer_struct.type = 0.0f;
	buffer_struct.intensity = 0.0f;

	needs_update = true;
}

void LightComponent::Draw(Render & ren)
{
	light_buffer->BindSlot(ren, 0);
}

void LightComponent::UpdateLightBuffer(Render & ren)
{
	if (light_buffer == nullptr)
		light_buffer = new ConstBuffer<LightBuffer>(ren, buffer_struct, BUFFER_TYPE::PIXEL_SHADER_BUFFER);
	else light_buffer->Update(ren, buffer_struct);

	needs_update = false;
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
		ImGui::ColorPicker3("##Light color picker", (float*)&buffer_struct.color, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_::ImGuiColorEditFlags_NoSidePreview);
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
