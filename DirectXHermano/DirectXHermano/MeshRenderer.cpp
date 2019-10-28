#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "ImGui\imgui.h"

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

	if (matrices == nullptr || ren.GetCamera().needs_update || parent->GetTransform()->NeedsUpdate())
	{
		matrix_buffers new_matrices =
		{
			{ parent->GetTransform()->GetMatrix() },
			{ DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(cam.position.x, cam.position.y, cam.position.z) * DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(cam.yaw, cam.pitch, cam.roll))) },
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

void MeshRenderer::DrawMeshRendererUI(ResourceManager& res_manager)
{
	if (ImGui::TreeNode("Mesh Renderer"))
	{
		ImGui::Text("Mesh:");
		ImGui::SameLine();
		if (mesh_to_draw)
		{
			if (ImGui::BeginCombo(" ", mesh_to_draw->GetName(), ImGuiComboFlags_::ImGuiComboFlags_None))
			{
				Mesh* selected_mesh = &res_manager.DrawMeshesUI();
				if (selected_mesh)
					mesh_to_draw = selected_mesh;
				ImGui::EndCombo();
			}

			ImGui::Text("Num Submeshes: %i \nNum Vertices: %i \nNum Indices: %i", mesh_to_draw->GetNumSubmeshes(), mesh_to_draw->GetNumAllVertices(), mesh_to_draw->GetNumAllIndices());
		}
		else
		{
			if (ImGui::BeginCombo(" " , "No Mesh Selected"))
			{

				Mesh* tmp = &res_manager.DrawMeshesUI();
				if (tmp != nullptr)
					mesh_to_draw = tmp;
				ImGui::EndCombo();
			}
		}

		

		//

		ImGui::TreePop();
	}
}

void MeshRenderer::Draw(Render & ren)
{
	if (mesh_to_draw)
	{
		PrepareMatrices(ren);
		mesh_to_draw->Draw(ren);
	}
}
