#include "LightComponent.h"
#include "ImGui/imgui.h"

LightComponent::LightComponent(LIGHT_TYPE type) : light_type(type)
{
	buffer_struct = {};
	buffer_struct.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	buffer_struct.type = (float)type;
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

void LightComponent::SetDirection(DirectX::XMFLOAT3 dir)
{
	if (light_type == DIRECTIONAL_LIGHT)
	{
		buffer_struct.direction = dir;
		needs_update = true;
	}
}

void LightComponent::SetPosition(DirectX::XMFLOAT3 pos)
{
	if (light_type == POINT_LIGHT)
	{
		buffer_struct.position = pos;
		needs_update = true;
	}
}

void LightComponent::DrawUI()
{
	if (ImGui::TreeNode("Light Component"))
	{
		ImGui::Text("Light Type: ");
		if (ImGui::BeginCombo("##Light combo", LightTypeToChar()))
		{
			if (ImGui::Selectable("Directional Light"))
			{
				buffer_struct.type = (float)LIGHT_TYPE::DIRECTIONAL_LIGHT;
				needs_update = true;
			}

			if (ImGui::Selectable("Point Light"))
			{
				buffer_struct.type = (float)LIGHT_TYPE::POINT_LIGHT;
				needs_update = true;
			}

			ImGui::EndCombo();
		}

		ImGui::Separator();

		ImGui::Text("Light Color: ");

		ImGui::PushItemWidth(120.0f);

		if(ImGui::ColorPicker3("##Light color picker", (float*)&buffer_struct.color, ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_::ImGuiColorEditFlags_NoSidePreview))
			needs_update = true;

		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}

const char* LightComponent::LightTypeToChar()
{
	switch ((int)buffer_struct.type)
	{
	case DIRECTIONAL_LIGHT:
		return "Directional Light";
		break;
	case POINT_LIGHT:
		return "Point Light";
		break;
	}
}
