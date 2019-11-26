#include <iostream>
#include "ResourceManager.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "ShaderProgram.h"
#include "ImGui/imgui.h"
#include "FileDrop.h"
#include "Texture.h"
#include "Sampler.h"
#include "Material.h"
#include "TextureResource.h"
#include "Preset.h"

//Assimp
#pragma comment(lib, "Assimp/libx86/assimp.lib")
#include "Assimp/include/scene.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

//image load
#define STB_IMAGE_IMPLEMENTATION
#include "STB Image/stb_image.h"

ResourceManager::~ResourceManager()
{
	for (std::multimap<RESOURCE_TYPE, Resource*>::iterator iter = mapped_resources.begin(); iter != mapped_resources.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	mapped_resources.erase(mapped_resources.begin(), mapped_resources.end());
}

void ResourceManager::Start(Render& ren)
{
	//Load Basic meshes
	LoadShaders(ren);
	LoadCube(ren);
	LoadPlane(ren);

	actual_resource_path = "C:/Users/Usuari/Documents/GitHub/CuteEngine/CuteEngine/Resources/Models/Patrick/Patrick.obj";
	ImportMesh(actual_resource_path, ren);

	//Load Null Texture
	LoadNullTexture(ren);
}

void ResourceManager::DrawMaterialEditorUI(Render& ren)
{
	ImGui::Begin("Material Editor");

	ImGui::Text("Material Editor: ");

	ImGui::InputTextWithHint(" ##material editor creator", "Insert Material Name", material_name_buffer, 128);

	if (ImGui::Button("Create Material"))
	{
		if (strlen(material_name_buffer) != 0)
		{
			Material* new_material = new Material(material_name_buffer);
			new_material->InitColorBuffer(ren);
			new_material->SetAlbedoTexture((TextureResource*)GetResourceByName("Null Texture", RESOURCE_TYPE::TEXTURE));

			mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(MATERIAL, new_material));
			material_to_modify = new_material;
		}
	}

	ImGui::Separator();

	if (material_to_modify != nullptr)
	{
		ImGui::Text("Diffuse: ");
		ID3D11ShaderResourceView* test = material_to_modify->GetTexture(TEXTURE_TYPE::ALBEDO);

		if(test)
			ImGui::Image(material_to_modify->GetTexture(TEXTURE_TYPE::ALBEDO), ImVec2(50.0f, 50.0f));

		ImGui::SameLine();
		if (ImGui::BeginCombo(" ##difuse texture selector", "Select Texture", ImGuiComboFlags_::ImGuiComboFlags_None))
		{
			const TextureResource* tmp = (TextureResource*)DrawResourceSelectableUI(TEXTURE);

			if (tmp != nullptr)
				material_to_modify->SetAlbedoTexture((TextureResource*)tmp);

			ImGui::EndCombo();
		}

		ImGui::Text("Color:"); ImGui::SameLine();

		ImGui::PushItemWidth(120.0f);
		if (ImGui::ColorPicker4(" ##difuse color", material_to_modify->GetColor(TEXTURE_TYPE::ALBEDO), ImGuiColorEditFlags_::ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_::ImGuiColorEditFlags_NoSidePreview))
			material_to_modify->UpdateColor(TEXTURE_TYPE::ALBEDO, ren);

		ImGui::PopItemWidth();
		ImGui::Separator();
	}
	ImGui::End();
}

void ResourceManager::ImportResource(const File* file, Render & ren)
{
	actual_resource_path = file->GetPath();
	switch (file->GetType())
	{
	case FILE_TYPE::FBX:
	case FILE_TYPE::OBJ:
		ImportMesh(actual_resource_path, ren);
		break;

	case FILE_TYPE::PNG:
	case FILE_TYPE::JPG:
		ImportTexture(actual_resource_path, ren);

		break;
	}
	
}

Material* ResourceManager::GetMaterialByName(std::string name) const
{
	std::multimap<RESOURCE_TYPE, Resource*>::const_iterator lower, up;

	lower = mapped_resources.lower_bound(RESOURCE_TYPE::MATERIAL);
	up = mapped_resources.upper_bound(RESOURCE_TYPE::MATERIAL);

	for (; lower != up; lower++)
	{
		if (name == lower->second->GetName())
			return (Material*)lower->second;
	}

	return nullptr;
}

void ResourceManager::SetMaterialToModify(Material * current_mat)
{
	if (current_mat != nullptr)
		material_to_modify = current_mat;
}

void ResourceManager::ImportMesh(const char* path, Render& ren)
{
	//new Mesh
	std::string name = path;
	Mesh* new_mesh = new Mesh(path);

	//Load Mesh from assimp
	Assimp::Importer importer;

	const aiScene* scene= importer.ReadFile(path, aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FixInfacingNormals |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PreTransformVertices |
		//aiProcess_RemoveRedundantMaterials |
		aiProcess_SortByPType |
		aiProcess_ImproveCacheLocality |
		aiProcess_FlipUVs |
		aiProcess_OptimizeMeshes);

	//Check for errors
	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		const char* tmp = importer.GetErrorString();
		std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	//Load each node from scene
	nodes.push(scene->mRootNode);

	Preset* new_preset = new Preset(actual_resource_path);
	while (!nodes.empty())
	{
		new_mesh->AddSubmesh(ProcessNode(scene, nodes.front(), ren));
		new_preset->AddMaterialName(ProcessMaterials(scene, nodes.front(), ren));

		for (int i = 0; i < nodes.front()->mNumChildren; i++)
			nodes.push(nodes.front()->mChildren[i]);

		nodes.pop();
	}
	mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(PRESET, new_preset));
	std::pair<RESOURCE_TYPE, Resource*> map_element = { RESOURCE_TYPE::MESH, new_mesh };
	mapped_resources.insert(map_element);
}

void ResourceManager::LoadShaders(Render& ren)
{
	mesh_shader = new ShaderProgram(ren, L"VertexShader.cso", L"PixelShader.cso");
	screen_shader = new ShaderProgram(ren, L"ScreenVertexShader.cso", L"ScreenPixelShader.cso");
	light_shader = new ShaderProgram(ren, L"ScreenVertexShader.cso", L"LightPixelShader.cso");
}

void ResourceManager::LoadCube(Render& ren)
{
	//Cube Vertices
	std::vector<Vertex> vertices =
	{
		//Back
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f} },
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { 1.0f,  1.0f, -1.0f },  { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, -1.0f, -1.0f },  { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },

		//Front
		{ { -1.0f, -1.0f,  1.0f }, {0.0f, 0.0f, 1.0f},  { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
		{ { 1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },

		//Right
		{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { 1.0f,  1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },

		//Left
		{ { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },

		//Up
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f,  1.0f, -1.0f },  { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { 1.0f,  1.0f,  1.0f },  { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },

		//Down
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { 1.0f, -1.0f, -1.0f },  { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f },  { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
	};

	//Cube Index Buffer
	std::vector<unsigned short> indices =
	{
		//Back
		0, 1, 2, 0, 2, 3,

		//Front
		7, 6, 5, 7, 5, 4,

		//Right
		9, 8, 10, 9, 10, 11,

		//Left
		14, 15, 13, 14, 13, 12,

		//Up
		16, 18, 19, 16, 19, 17,

		//Down
		20, 21, 23, 20, 23, 22
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoords", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VertexBuffer* new_vertexbuffer = new VertexBuffer(ren, vertices);
	IndexBuffer* new_indexbuffer = new IndexBuffer(ren, indices);
	Topology* new_topology = new Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InputLayout* new_inputlayout = new InputLayout(ren, layout, mesh_shader->GetVertexByteCode());

	Submesh* new_submesh = new Submesh();

	new_submesh->AddBind(new_vertexbuffer);
	new_submesh->AddBind(new_topology);
	new_submesh->AddBind(new_inputlayout);
	new_submesh->AddIndices(new_indexbuffer);
	new_submesh->AddInfo(vertices.size(), indices.size());

	//std::string name = "Cube Mesh";
	Mesh* new_mesh = new Mesh((std::string)"/Cube Mesh");
	new_mesh->AddSubmesh(new_submesh);

	//IMAGE TESST
	//actual_resource_path = "C:/Users/Usuari/Desktop/unknown.png";
	actual_resource_path = "C:/Users/Th_Sola/Desktop/GameDev/uri.png";
	Texture* test_text = ImportImage("uri.png", ren);
	Sampler* test_sampler = new Sampler(ren);

	//TextureResource* harambe_texture = new TextureResource("/Harambe.png");
	TextureResource* harambe_texture = new TextureResource("/Uri.png");
	harambe_texture->AddTexture(test_text);
	harambe_texture->AddSampler(test_sampler);

	Material* harambe_material = new Material("/Harambe_Material");
	harambe_material->InitColorBuffer(ren);
	harambe_material->SetAlbedoTexture(harambe_texture);

	std::pair<RESOURCE_TYPE, Resource*> map_element_mesh = { RESOURCE_TYPE::MESH, new_mesh };
	std::pair<RESOURCE_TYPE, Resource*> map_element_texture = { RESOURCE_TYPE::TEXTURE, harambe_texture };
	std::pair<RESOURCE_TYPE, Resource*> map_element_material = { RESOURCE_TYPE::MATERIAL, harambe_material };

	mapped_resources.insert(map_element_mesh);
	mapped_resources.insert(map_element_texture);
	mapped_resources.insert(map_element_material);
}

void ResourceManager::LoadNullTexture(Render& ren)
{
	TextureResource* null_texture = new TextureResource("/Null Texture");

	Texture* null_text = new Texture(ren, nullptr, 0, 0, 0);
	null_texture->AddTexture(null_text);

	mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(TEXTURE, null_texture));
}

void ResourceManager::LoadPlane(Render & ren)
{
	std::vector<Vertex> plane_vertexs = 
	{
		{ { -1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -1.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ {  1.0f,  1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f,  0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	};

	std::vector<unsigned short> indices = { 3, 2, 1, 3, 1, 0 };

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TextureCoords", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VertexBuffer* new_vertexbuffer = new VertexBuffer(ren, plane_vertexs);
	IndexBuffer* new_indexbuffer = new IndexBuffer(ren, indices);
	Topology* new_topology = new Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InputLayout* new_inputlayout = new InputLayout(ren, layout, screen_shader->GetVertexByteCode());

	Submesh* new_submesh = new Submesh();

	new_submesh->AddBind(new_vertexbuffer);
	new_submesh->AddBind(new_topology);
	new_submesh->AddBind(new_inputlayout);
	new_submesh->AddIndices(new_indexbuffer);
	new_submesh->AddInfo(plane_vertexs.size(), indices.size());

	//std::string name = "Cube Mesh";
	Mesh* new_mesh = new Mesh((std::string)"/Plane Mesh");
	new_mesh->AddSubmesh(new_submesh);

	screen_mesh = new_mesh;

	mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(RESOURCE_TYPE::MESH, new_mesh));

}

std::vector<Submesh*> ResourceManager::ProcessNode(const aiScene* scene, aiNode* node, Render& ren)
{
	std::vector<Submesh*> ret;
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ret.push_back(ProcessMesh(scene, mesh, ren));
	}

	return ret;
}

Submesh* ResourceManager::ProcessMesh(const aiScene * scene, aiMesh * mesh, Render& ren)
{
	Submesh* new_submesh = new Submesh();
	std::vector<Vertex> vertices;
	bool has_text_coords = false;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//Vertex position / Normals / Tex Coords
		Vertex new_vertex;
		new_vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		new_vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

		if (mesh->mTextureCoords[0])
		{
			has_text_coords = true;
			new_vertex.texture_coords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}

		vertices.push_back(new_vertex);
	}

	std::vector<unsigned short> indices;
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace poligon_face = mesh->mFaces[i];

		for (int k = 0; k < poligon_face.mNumIndices; k++)
			indices.push_back(poligon_face.mIndices[k]);
	}
	std::cout << indices.size() << std::endl;

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (has_text_coords)
		layout.push_back({ "TextureCoords", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	VertexBuffer* new_vertexbuffer = new VertexBuffer(ren, vertices);
	IndexBuffer* new_indexbuffer = new IndexBuffer(ren, indices);
	Topology* new_topology = new Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InputLayout* new_inputlayout = new InputLayout(ren, layout, mesh_shader->GetVertexByteCode());

	new_submesh->AddBind(new_vertexbuffer);
	new_submesh->AddIndices(new_indexbuffer);
	new_submesh->AddBind(new_topology);
	new_submesh->AddBind(new_inputlayout);
	new_submesh->AddInfo(vertices.size(), indices.size());

	return new_submesh;
}

void ResourceManager::ImportTexture(const char * path, Render & ren)
{
	//Check if the Texture is already loaded
	TextureResource* new_texture = (TextureResource*)GetResourceByPath(actual_resource_path, TEXTURE);

	if (new_texture != nullptr)
		return;

	//Check if there is problems with the importation
	Texture* tmp = ImportImage(nullptr, ren);
	if (tmp != nullptr)
	{
		//Create Texture
		new_texture = new TextureResource(actual_resource_path);
		Sampler* new_sampler = new Sampler(ren);

		new_texture->AddTexture(tmp);
		new_texture->AddSampler(new_sampler);

		if (new_texture != nullptr)
			mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(TEXTURE, new_texture));
	}
}

Texture* ResourceManager::ImportImage(const char * path, Render & ren)
{
	//IMAGE load
	std::string full_texture_path = actual_resource_path;
	
	if (path != nullptr)
	{
		if (full_texture_path.find_last_of('/') != -1)
			full_texture_path.erase(full_texture_path.find_last_of('/') + 1);
		else full_texture_path.erase(full_texture_path.find_last_of('\\') + 1);

		full_texture_path.append(path);
	}

	int width, height, color_channels;
	unsigned char* data = stbi_load(full_texture_path.c_str(), &width, &height, &color_channels, 4);

	if (data != nullptr)
	{
		Texture* texture = new Texture(ren, data, width, height, color_channels);
		return texture;
	}

	return nullptr;
}

const char* ResourceManager::ProcessMaterials(const aiScene * scene, aiNode * node, Render & ren)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		
		//Load Materials
		if (scene->HasMaterials())
		{
			aiMaterial* mesh_material = scene->mMaterials[mesh->mMaterialIndex];

			//Create Material
			aiString material_name;
			aiGetMaterialString(mesh_material, AI_MATKEY_NAME, &material_name);

			Material* new_material = new Material(material_name.C_Str());
			new_material->InitColorBuffer(ren);

			//Look For Difuse Texture 
			aiString texture_path;
			if (aiGetMaterialTexture(mesh_material, aiTextureType_DIFFUSE, 0, &texture_path) == aiReturn_SUCCESS)
			{
				//Check if the texture is already loaded
				TextureResource* new_texture = (TextureResource*)GetResourceByName(texture_path.C_Str(), TEXTURE);
				if (new_texture == nullptr)
				{
					//Load the Texture
					new_texture = new TextureResource(texture_path.C_Str());
					new_texture->AddTexture(ImportImage(texture_path.C_Str(), ren));
					new_texture->AddSampler(new Sampler(ren));

					//Add the texture to resources
					mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(TEXTURE, new_texture));
				}
				//Add the albedo to the Material
				new_material->SetAlbedoTexture(new_texture);
			}

			//Look for Difuse Color
			aiColor4D difuse_color;
			if (aiGetMaterialColor(mesh_material, AI_MATKEY_COLOR_DIFFUSE, &difuse_color) == aiReturn_SUCCESS)
			{
				new_material->SetAlbedoColor(ren, difuse_color.r, difuse_color.g, difuse_color.b, difuse_color.a);
			}

			//Add Material and Preset to the Resources map
			mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(MATERIAL, new_material));
			return new_material->GetName();
		}
	}
}

Resource* ResourceManager::GetResourceByPath(const char * full_path, RESOURCE_TYPE type)
{
	std::multimap<RESOURCE_TYPE, Resource*>::const_iterator lower, up;

	lower = mapped_resources.lower_bound(type);
	up = mapped_resources.upper_bound(type);

	for (; lower != up; lower++)
	{
		if (lower->second->GetPath() == full_path)
			return lower->second;

	}
	return nullptr;
}

Resource* ResourceManager::GetResourceByName(const char * name, RESOURCE_TYPE type)
{
	std::multimap<RESOURCE_TYPE, Resource*>::const_iterator lower, up;

	lower = mapped_resources.lower_bound(type);
	up = mapped_resources.upper_bound(type);

	for (; lower != up; lower++)
	{
		if (strcmp(lower->second->GetName(), name) == 0)
			return lower->second;

	}
	return nullptr;
}

Resource* ResourceManager::DrawResourceSelectableUI(RESOURCE_TYPE type)
{
	std::multimap<RESOURCE_TYPE, Resource*>::const_iterator lower, up;

	lower = mapped_resources.lower_bound(type);
	up = mapped_resources.upper_bound(type);

	for (; lower != up; lower++)
	{
		if (ImGui::Selectable(lower->second->GetName()))
			return lower->second;
		
	}
	return nullptr;
}
