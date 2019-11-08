#include "Entity.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "ImGui\imgui.h"
#include "Material.h"
#include "Preset.h"

MeshRenderer::MeshRenderer(Entity* _parent) : parent(_parent), mesh_to_draw(nullptr)
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
		//Meshrenderer MESH UI 
		ImGui::Separator();
		ImGui::Text("Mesh:");
		if (mesh_to_draw != nullptr)
		{
			if (ImGui::BeginCombo(" ", mesh_to_draw->GetName(), ImGuiComboFlags_::ImGuiComboFlags_None))
			{
				const Mesh* selected_mesh = res_manager.DrawMeshesUI();
				if (selected_mesh)
					mesh_to_draw = (Mesh*)selected_mesh;
				ImGui::EndCombo();
			}

			ImGui::Text("Num Submeshes: %i \nNum Vertices: %i \nNum Indices: %i", mesh_to_draw->GetNumSubmeshes(), mesh_to_draw->GetNumAllVertices(), mesh_to_draw->GetNumAllIndices());
		}
		else
		{
			if (ImGui::BeginCombo(" " , "No Mesh Selected"))
			{

				const Mesh* tmp = res_manager.DrawMeshesUI();
				if (tmp != nullptr)
					mesh_to_draw = (Mesh*)tmp;
				ImGui::EndCombo();
			}
		}

		//Meshrenderer MATERIAL UI
		ImGui::Separator();
		ImGui::Text("Material");

		for (int i = 0; i < meshes_materials.size(); i++)
		{
			if (ImGui::Selectable(meshes_materials[i]->GetName()))
				res_manager.SetMaterialToModify(meshes_materials[i]);		
		}
		

		if (ImGui::BeginCombo(" ##material", "Add Material", ImGuiComboFlags_::ImGuiComboFlags_None))
		{
			const Material* tmp = res_manager.DrawMaterialUI();
			AddMaterial((Material*)tmp);

			ImGui::EndCombo();
		}

		if (ImGui::Button("Erase Material"))
		{
			if(!meshes_materials.empty())
				meshes_materials.pop_back();
		}

		ImGui::SameLine();

		if (ImGui::Button("Delete All Materials"))
			meshes_materials.clear();


		//Meshrenderer Presets UI
		ImGui::Separator();
		ImGui::Text("Load Preset");
		if (ImGui::BeginCombo(" ##preset", "Load Preset", ImGuiComboFlags_::ImGuiComboFlags_None))
		{
			const Preset* tmp = res_manager.DrawPresetsUI();
			
			if (tmp != nullptr)
			{
				LoadPreset(*(Preset*)tmp, res_manager);
			}

			ImGui::EndCombo();
		}

		ImGui::TreePop();
	}
}

void MeshRenderer::AddMaterial(Material * new_mat)
{
	if (new_mat)
		meshes_materials.push_back(new_mat);
}

void MeshRenderer::Draw(Render & ren)
{
	if (mesh_to_draw != nullptr)
	{
		PrepareMatrices(ren);

		//Same number of meshes and materials
		for (int i = 0; i < mesh_to_draw->GetNumSubmeshes(); i++)
		{
			if(!meshes_materials.empty() && i < meshes_materials.size())
				meshes_materials[i]->BindTexture(ren, TEXTURE_TYPE::ALBEDO);

			mesh_to_draw->DrawSubMesh(ren, i);
		}
	}
}

void MeshRenderer::LoadPreset(Preset& preset, ResourceManager& resources)
{
	//Reset array
	meshes_materials.clear();

	//Load new materials
	for (int i = 0; i < preset.GetNumMaterials(); i++)
	{
		AddMaterial(resources.GetMaterialByName(preset.GetMaterialAt(i)));
	}
}