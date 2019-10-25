#pragma once
#include "Bindable.h"
#include <vector>
#include "Resource.h"

class IndexBuffer;

class Submesh
{
public:
	Submesh() = default;
	~Submesh();

	void Draw(Render& ren);

	void AddBind(const Bindable* bind);
	void AddIndices(const IndexBuffer* bind);

private:
	const IndexBuffer* indexes = nullptr;
	std::vector<Bindable*> binders;
};

class Mesh : public Resource
{
public:
	Mesh(std::string _name);
	Mesh(const char* path);
	~Mesh();

	void Draw(Render& ren);

	void AddSubmesh(const Submesh*);
	void AddSubmesh(std::vector<Submesh*>);

private:
	std::vector<Submesh*> meshes;
};