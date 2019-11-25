#include "Entity.h"
#include "Camera.h"
#include "ImGui\imgui.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "LightComponent.h"

//----------------------------------- TRANSFORM ------------------------------------------------
Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation_euler(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation_quaternion(DirectX::XMQuaternionRotationRollPitchYaw(rotation_euler.x, rotation_euler.y, rotation_euler.z))
{
	BuildMatrix();
}

void Transform::Update()
{
	if (needs_update)
	{
		BuildMatrix();
	}
}

void Transform::BuildMatrix()
{
	rotation_quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation_euler.x, rotation_euler.y, rotation_euler.z);

	model_matrix = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationQuaternion(rotation_quaternion) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * 
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		);
}

void Transform::DrawTransformUI()
{
	if(ImGui::TreeNode("Transform"))
	{
		ImGui::Text("Position ");
		ImGui::PushItemWidth(50);
		if (ImGui::DragFloat("X##position", &position.x, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##position", &position.y, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##position", &position.z, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;

		ImGui::Text("Rotation ");
		if (ImGui::DragFloat("X##rotation", &rotation_euler.x, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##rotation", &rotation_euler.y, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##rotation", &rotation_euler.z, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;

		ImGui::Text("Scale ");
		if (ImGui::DragFloat("X##scale", &scale.x, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Y##scale", &scale.y, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
		ImGui::SameLine();
		if (ImGui::DragFloat("Z##scale", &scale.z, 0.5f, 0.0f, 0.0f, "%.2f")) needs_update = true;
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
	transform->Update();
}

void Entity::Draw(Render & ren)
{
	mesh_render->Draw(ren);
}

void Entity::Delete()
{
	needs_delete = true;
}

void Entity::DrawUI(ResourceManager& res)
{
	ImGui::Text(name.c_str());

	transform->DrawTransformUI();
	mesh_render->DrawMeshRendererUI(res);

	if (!light_component)
	{
		if (ImGui::Button("Add Light Component"))
		{
			light_component = new LightComponent(LIGHT_TYPE::DIRECTIONAL_LIGHT);
			is_light = true;
		}
	}
}


