#pragma once
#include <vector>

class Render;
class Resource;
class ShaderProgram;
class Mesh;

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager();

	void Start(Render& ren);
	void Update();

	Mesh& GetCube() const { return (*cube_mesh); }
	ShaderProgram& GetShader() const { return (*mesh_shader); }

private:
	std::vector<Resource*> resources;

	//Test
	ShaderProgram* mesh_shader = nullptr;
	Mesh* cube_mesh = nullptr;

	void LoadShaders(Render& ren);
	void LoadCube(Render& ren);
};