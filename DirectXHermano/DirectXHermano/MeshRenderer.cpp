#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Camera.h"

MeshRenderer::MeshRenderer(Entity* _parent) : parent(_parent)
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

	if (ren.GetCamera().needs_update || parent->GetTransform()->NeedsUpdate())
	{
		matrix_buffers new_matrices =
		{
			{ parent->GetTransform()->GetMatrix() },
			{ DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(cam.position.x, cam.position.y, cam.position.z) * DirectX::XMMatrixRotationRollPitchYaw(cam.yaw, cam.pitch, cam.roll)) },
			{ DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(cam.fov, cam.aspect_ratio, cam.z_near, cam.z_far)) }
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
