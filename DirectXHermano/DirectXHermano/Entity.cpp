#include "Entity.h"
#include "Camera.h"
#include "ImGui\imgui.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "LightComponent.h"

//----------------------------------- TRANSFORM ------------------------------------------------
Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation_euler(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f)
{
	BuildMatrix();
}

bool Transform::Update()
{
	if (needs_update)
	{
		BuildMatrix();
		return true;
	}

	return false;
}

void Transform::BuildMatrix()
{
	DirectX::XMStoreFloat4(&rotation_quaternion, DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(rotation_euler.x), DirectX::XMConvertToRadians(rotation_euler.y), DirectX::XMConvertToRadians(rotation_euler.z)));
	DirectX::XMStoreFloat4x4(&model_matrix,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotation_quaternion)) *
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		)
	);	
}

void Transform::DrawTransformUI()
{
	if(ImGui::TreeNode("Transform"))
	{
		ImGui::Text("Position ");
		ImGui::PushItemWidth(50);
		if (ImGui::DragFloat("X##position", &position.x, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##position", &position.y, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##position", &position.z, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;

		ImGui::Text("Rotation ");
		if (ImGui::DragFloat("X##rotation", &rotation_euler.x, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##rotation", &rotation_euler.y, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##rotation", &rotation_euler.z, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;

		ImGui::Text("Scale ");
		if (ImGui::DragFloat("X##scale", &scale.x, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##scale", &scale.y, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##scale", &scale.z, 0.05f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}
}

//----------------------------------- TRANSFORM ------------------------------------------------

//----------------------------------- MESH RENDERER ---------------------------------------------


//-----------------------------------  ENTITY   -------------------------------------------------

Entity::Entity(int id)
{
	transform = new Transform();
	mesh_render = new MeshRenderer(this);

	char buffer[50];
	sprintf_s(buffer, "Entity_%i", id);
	name = buffer;
}

Entity::~Entity()
{
	if (transform)
	{
		delete transform;
		transform = nullptr;
	}
}

void Entity::Update()
{
	if (transform->Update())
	{
		if (light_component)
		{
			light_component->SetDirection(transform->GetRotation());
			light_component->SetPosition(transform->GetPosition());
		}
	}


}

void Entity::Draw(Render & ren)
{
	mesh_render->Draw(ren);
}

void Entity::DrawLight(Render & ren)
{
	if (light_component)
	{
		if (light_component->NeedsUpdate())
			light_component->UpdateLightBuffer(ren);

		mesh_render->PrepareMatrices(ren);
		light_component->Draw(ren);
	}
}

void Entity::Delete()
{
	needs_delete = true;
}

void Entity::CreateLightComponent()
{
	light_component = new LightComponent(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	light_component->SetDirection(transform->GetRotation());
	light_component->SetPosition(transform->GetPosition());

	is_light = true;
}

int Entity::GetLightType() const
{
	if (light_component)
		return	light_component->GetLightType();

	return -1;
}

void Entity::DrawUI(ResourceManager& res)
{
	ImGui::Text(name.c_str());

	transform->DrawTransformUI();
	ImGui::Separator();

	mesh_render->DrawMeshRendererUI(res);

	ImGui::Separator();
	if (!light_component)
	{
		if (ImGui::Button("     Add Light Component     "))
			CreateLightComponent();
	}
	else light_component->DrawUI();
}


