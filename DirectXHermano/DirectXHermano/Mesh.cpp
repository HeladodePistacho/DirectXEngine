#include "Mesh.h"
#include "IndexBuffer.h"

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

//--------------------------- SUBMESH ------------------------

//---------------------------  MESH   ------------------------
Mesh::Mesh(std::string _name) : Resource(RESOURCE_TYPE::MESH, _name)
{
}

Mesh::Mesh(const char* path) : Resource(RESOURCE_TYPE::MESH, path)
{

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


