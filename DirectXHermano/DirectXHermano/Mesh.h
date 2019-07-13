#pragma once
#include "Bindable.h"
#include <vector>

class IndexBuffer;

class Submesh
{
	Submesh();

	void AddBind(const Bindable& bind);
	void AddIndices(const IndexBuffer& bind);

private:
	IndexBuffer* indexes = nullptr;
	std::vector<Bindable*> binders;
};

class Mesh
{
	Mesh(const char* path);
	~Mesh();

private:
	std::vector<Submesh*> meshes;
};