#include "Entity.h"
#include "Mesh.h"
#include "Camera.h"
#include "ImGui\imgui.h"

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
	model_matrix = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationQuaternion(rotation_quaternion) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
	);
}

//----------------------------------- TRANSFORM ------------------------------------------------

//----------------------------------- MESH RENDERER ---------------------------------------------
MeshRenderer::MeshRenderer()
{

}

MeshRenderer::~MeshRenderer()
{
	if (mesh_to_draw)
		mesh_to_draw = nullptr;

	if (matrices)
	{
		delete matrices;
		matrices = nullptr;
	}
}

void MeshRenderer::PrepareMatrices(Render& ren)
{
	Camera cam = ren.GetCamera();

	if (ren.GetCamera().needs_update)
	{
		matrix_buffers new_matrices = 
		{
			{DirectX::XMMatrixIdentity()},
			{DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(cam.position.x, cam.position.y, cam.position.z) * DirectX::XMMatrixRotationRollPitchYaw(cam.yaw, cam.pitch, cam.roll))},
			{DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(cam.fov, cam.aspect_ratio, cam.z_near, cam.z_far))}
		};

		if (!matrices)
			matrices = new ConstBuffer<matrix_buffers>(ren, new_matrices, BUFFER_TYPE::VERTEX_SHADER_BUFFER); 
		else {
			matrices->Update(ren, new_matrices);
		}
	}

	matrices->Bind(ren);
}

void MeshRenderer::Draw(Render & ren)
{
	if (mesh_to_draw)
	{
		PrepareMatrices(ren);
		mesh_to_draw->Draw(ren);
	}
}


//-----------------------------------  ENTITY   -------------------------------------------------

Entity::Entity(int id)
{
	transform = new Transform();
	mesh_render = new MeshRenderer();

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
}

void Entity::Draw(Render & ren)
{
	mesh_render->Draw(ren);
}

void Entity::Delete()
{
	needs_delete = true;
}

void Entity::DrawUI()
{
	ImGui::Text("I am %s", name.c_str());
}


