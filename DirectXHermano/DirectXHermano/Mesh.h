#pragma once
#include "Bindable.h"
#include <vector>
#include "Resource.h"

class IndexBuffer;

class Texture;
class Sampler;

class Submesh
{
public:
	Submesh() = default;
	~Submesh();

	void Draw(Render& ren);

	void AddBind(const Bindable* bind);
	void AddIndices(const IndexBuffer* bind);

	int GetNumVertices() const;
	int GetNumIndices() const;
	void AddInfo(int vertices, int indices);

private:
	int num_vertices = 0;
	int num_indices = 0;
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

	int GetNumSubmeshes() const;
	int GetNumAllVertices() const;
	int GetNumAllIndices() const;

	//Test
	Texture* texture = nullptr;
	Sampler* sampler = nullptr;

private:
	std::vector<Submesh*> meshes;
};