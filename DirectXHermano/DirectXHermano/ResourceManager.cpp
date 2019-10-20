#include "ResourceManager.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "ShaderProgram.h"
#include "ImGui/imgui.h"


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
}

Mesh& ResourceManager::DrawMeshesUI()
{
	for (std::vector<Resource*>::iterator iter = resources.begin(); iter != resources.end(); iter++)
	{
		if ((*iter)->GetType() == RESOURCE_TYPE::MESH)
		{
			if (ImGui::Selectable((*iter)->GetName()))
			{
				return *(Mesh*)(*iter);
			}
		}
	}
}

void ResourceManager::ImportMesh(const char* path)
{
	Mesh* new_mesh = new Mesh(path);

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
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f }
	};

	//Cube Index Buffer
	std::vector<unsigned short> indices = 
	{
		//Back
		0, 1, 2, 0, 2, 3,

		//Right
		3, 2, 6, 3, 6, 7,

		//Front
		7, 6, 5, 7, 5, 4,

		//Left
		4, 5, 1, 4, 1, 0,

		//Up
		2, 1, 5, 2, 5, 6,

		//Down
		3, 7, 4, 3, 4, 0
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	Mesh* new_mesh = new Mesh((std::string)"Cube Mesh");
	new_mesh->AddSubmesh(new_submesh);
	
	resources.push_back(new_mesh);

	//Test
	cube_mesh = new_mesh;
}

