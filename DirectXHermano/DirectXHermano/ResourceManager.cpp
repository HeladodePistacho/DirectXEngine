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
	actual_resource_path = "C:/Users/Usuari/Documents/GitHub/CuteEngine/CuteEngine/Resources/Models/Patrick/Patrick.obj";
	ImportMesh(actual_resource_path, ren);
}

Mesh& ResourceManager::DrawMeshesUI()
{
	std::multimap<RESOURCE_TYPE, Resource*>::iterator lower, up;

	lower = mapped_resources.lower_bound(RESOURCE_TYPE::MESH);
	up = mapped_resources.upper_bound(RESOURCE_TYPE::MESH);

	for (; lower != up; lower++)
	{		
		if (ImGui::Selectable(lower->second->GetName()))
		{
			return *(Mesh*)lower->second;
		}
	}
}

Material& ResourceManager::DrawMaterialUI()
{
	std::multimap<RESOURCE_TYPE, Resource*>::iterator lower, up;

	lower = mapped_resources.lower_bound(RESOURCE_TYPE::MATERIAL);
	up = mapped_resources.upper_bound(RESOURCE_TYPE::MATERIAL);

	for (; lower != up; lower++)
	{
		if (ImGui::Selectable(lower->second->GetName()))
		{
			return *(Material*)lower->second;
		}
	}
}

Preset & ResourceManager::DrawPresetsUI()
{
	std::multimap<RESOURCE_TYPE, Resource*>::iterator lower, up;

	lower = mapped_resources.lower_bound(RESOURCE_TYPE::PRESET);
	up = mapped_resources.upper_bound(RESOURCE_TYPE::PRESET);

	for (; lower != up; lower++)
	{
		if (ImGui::Selectable(lower->second->GetName()))
		{
			return *(Preset*)lower->second;
		}
	}
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
		{{ -1.0f, -1.0f,  1.0f }, {0.0f, 0.0f, 1.0f},  { 1.0f, 1.0f } },
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
	actual_resource_path = "C:/Users/Usuari/Desktop/unknown.png";
	Texture* test_text = ImportImage("unknown.png", ren);
	Sampler* test_sampler = new Sampler(ren);

	TextureResource* harambe_texture = new TextureResource("/Harambe.png");
	harambe_texture->AddTexture(test_text);
	harambe_texture->AddSampler(test_sampler);

	Material* harambe_material = new Material("/Harambe_Material");
	harambe_material->SetAlbedoTexture(harambe_texture);

	std::pair<RESOURCE_TYPE, Resource*> map_element_mesh = { RESOURCE_TYPE::MESH, new_mesh };
	std::pair<RESOURCE_TYPE, Resource*> map_element_texture = { RESOURCE_TYPE::TEXTURE, harambe_texture };
	std::pair<RESOURCE_TYPE, Resource*> map_element_material = { RESOURCE_TYPE::MATERIAL, harambe_material };

	mapped_resources.insert(map_element_mesh);
	mapped_resources.insert(map_element_texture);
	mapped_resources.insert(map_element_material);
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

Texture* ResourceManager::ImportImage(const char * path, Render & ren)
{
	//IMAGE load
	std::string full_texture_path = actual_resource_path;
	
	if (full_texture_path.find_last_of('/') != -1)
		full_texture_path.erase(full_texture_path.find_last_of('/') + 1);
	else full_texture_path.erase(full_texture_path.find_last_of('\\') + 1);
		
	full_texture_path.append(path);
	
	int width, height, color_channels;
	unsigned char* data = stbi_load(full_texture_path.c_str(), &width, &height, &color_channels, 4);

	if (data != nullptr)
	{
		Texture* texture = new Texture(ren, data, width, height, color_channels);
		return texture;
	}
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

			//Look For Difuse Texture
			aiString texture_path;
			if (aiGetMaterialTexture(mesh_material, aiTextureType_DIFFUSE, 0, &texture_path) == aiReturn_SUCCESS)
			{
				//Load the Texture
				TextureResource* new_texture = new TextureResource(texture_path.C_Str());
				new_texture->AddTexture(ImportImage(texture_path.C_Str(), ren));
				new_texture->AddSampler(new Sampler(ren));

				//Add the albedo to the Material
				new_material->SetAlbedoTexture(new_texture);

				//Add the texture to resources
				mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(TEXTURE, new_texture));
			}

			//Add Material and Preset to the Resources map
			mapped_resources.insert(std::pair<RESOURCE_TYPE, Resource*>(MATERIAL, new_material));
			return new_material->GetName();
		}
	}
}
