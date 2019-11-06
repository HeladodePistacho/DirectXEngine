#include "Mesh.h"
#include "IndexBuffer.h"

//#pragma comment(lib, "assimpd.lib")
#pragma comment(lib, "Assimp/libx86/assimp.lib")
#include "Assimp/include/scene.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/mesh.h"

//Test
#include "Texture.h"
#include "Sampler.h"

//--------------------------- SUBMESH ------------------------
Submesh::~Submesh()
{
	for (int i = 0; i < binders.size(); i++)
	{
		delete binders[i];
		binders[i] = nullptr;
	}

	binders.erase(binders.begin(), binders.end());
}

void Submesh::Draw(Render & ren)
{
	for (int i = 0; i < binders.size(); i++)
	{
		binders[i]->Bind(ren);
	}

	ren.DrawIndices(indexes->GetNumIndices());
}

void Submesh::AddBind(const Bindable* bind)
{
	if(bind)
		binders.push_back((Bindable*)bind);
}

void Submesh::AddIndices(const IndexBuffer* bind)
{
	if (bind)
	{
		indexes = bind;
		binders.push_back((Bindable*)indexes);
	}
}

int Submesh::GetNumVertices() const
{
	return num_vertices;
}

int Submesh::GetNumIndices() const
{
	return num_indices;
}

void Submesh::AddInfo(int vertices, int indices)
{
	num_vertices = vertices;
	num_indices = indices;
}

//--------------------------- SUBMESH ------------------------

//---------------------------  MESH   ------------------------
Mesh::Mesh(std::string _name) : Resource(RESOURCE_TYPE::MESH, _name)
{
}

Mesh::Mesh(const char* path) : Resource(RESOURCE_TYPE::MESH, path)
{
	//Load Mesh
	//Assimp::Importer importer;
	//
	//fopen(path, )
}

Mesh::~Mesh()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
		meshes[i] = nullptr;
	}

	meshes.erase(meshes.begin(), meshes.end());
}

void Mesh::Draw(Render & ren)
{
	if (texture)
		texture->Bind(ren);

	if (sampler)
		sampler->Bind(ren);

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(ren);
	}
}

void Mesh::AddSubmesh(const Submesh* new_submesh)
{
	if (new_submesh)
		meshes.push_back((Submesh*)new_submesh);
}

void Mesh::AddSubmesh(std::vector<Submesh*> new_submeshes)
{
	for (std::vector<Submesh*>::iterator item = new_submeshes.begin(); item != new_submeshes.end(); item++)
	{
		if (*item)
			meshes.push_back((*item));
	}
}

int Mesh::GetNumSubmeshes() const
{
	return meshes.size();
}

int Mesh::GetNumAllVertices() const
{
	int vertices = 0;

	for (int i = 0; i < meshes.size(); i++)
		vertices += meshes[i]->GetNumVertices();

	return vertices;
}

int Mesh::GetNumAllIndices() const
{
	int indices = 0;

	for (int i = 0; i < meshes.size(); i++)
		indices += meshes[i]->GetNumIndices();

	return indices;
}

