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

//Assimp
#pragma comment(lib, "Assimp/libx86/assimp.lib")
#include "Assimp/include/scene.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < resources.size(); i++)
	{
		delete resources[i];
		resources[i] = nullptr;
	}

	resources.erase(resources.begin(), resources.end());
}

void ResourceManager::Start(Render& ren)
{
	//Load Basic meshes
	LoadShaders(ren);
	LoadCube(ren);
	ImportMesh("C:/Users/Usuari/Documents/GitHub/CuteEngine/CuteEngine/Resources/Models/Patrick/Patrick.obj", ren);
}

Mesh& ResourceManager::DrawMeshesUI()
{
	for (std::vector<Resource*>::iterator iter = resources.begin(); iter != resources.end(); iter++)
	{
		if ((*iter)->GetType() == RESOURCE_TYPE::MESH)
		{
			if (ImGui::Selectable((*iter)->GetName()))
				return *(Mesh*)(*iter);
			
		}
	}
}

void ResourceManager::ImportResource(const File* file, Render & ren)
{
	switch (file->GetType())
	{
	case FILE_TYPE::FBX:
	case FILE_TYPE::OBJ:
		ImportMesh(file->GetPath(), ren);
		break;

	case FILE_TYPE::PNG:

		break;
	}
	
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
	while (!nodes.empty())
	{
		new_mesh->AddSubmesh(ProcessNode(scene, nodes.front(), ren));

		for (int i = 0; i < nodes.front()->mNumChildren; i++)
			nodes.push(nodes.front()->mChildren[i]);

		nodes.pop();
	}

	resources.push_back(new_mesh);
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
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } },
		{ { 1.0f,  1.0f, -1.0f },  { 0.0f, 0.0f, -1.0f } },
		{ { 1.0f, -1.0f, -1.0f },  { 0.0f, 0.0f, -1.0f } },

		//Front
		{{ -1.0f, -1.0f,  1.0f }, {0.0f, 0.0f, 1.0f}},
		{{ -1.0f,  1.0f,  1.0f }, {0.0f, 0.0f, 1.0f}},
		{{  1.0f,  1.0f,  1.0f }, {0.0f, 0.0f, 1.0f}},
		{{  1.0f, -1.0f,  1.0f }, {0.0f, 0.0f, 1.0f}},

		//Right
		{ { 1.0f,  1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f,  1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f } },

		//Left
		{ { -1.0f, -1.0f, -1.0f },{ -1.0f, 0.0f, 0.0f } },
		{ { -1.0f,  1.0f, -1.0f },{ -1.0f, 0.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f },{ -1.0f, 0.0f, 0.0f } },
		{ { -1.0f,  1.0f,  1.0f },{ -1.0f, 0.0f, 0.0f } },

		//Up
		{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { 1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { -1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { 1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f } },

		//Down
		{ { -1.0f, -1.0f, -1.0f },{ 0.0f, -1.0f, 0.0f } },
		{ { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f },{ 0.0f, -1.0f, 0.0f } },
		{ { 1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f } },
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
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	//std::string name = "Cube Mesh";
	Mesh* new_mesh = new Mesh((std::string)"/Cube Mesh");
	new_mesh->AddSubmesh(new_submesh);
	
	resources.push_back(new_mesh);

	//Test
	cube_mesh = new_mesh;
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

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex new_vertex;
		new_vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		new_vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		//Add more stuff such as Normals/TexCoords...
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
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	VertexBuffer* new_vertexbuffer = new VertexBuffer(ren, vertices);
	IndexBuffer* new_indexbuffer = new IndexBuffer(ren, indices);
	Topology* new_topology = new Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	InputLayout* new_inputlayout = new InputLayout(ren, layout, mesh_shader->GetVertexByteCode());

	new_submesh->AddBind(new_vertexbuffer);
	new_submesh->AddIndices(new_indexbuffer);
	new_submesh->AddBind(new_topology);
	new_submesh->AddBind(new_inputlayout);

	return new_submesh;
}
