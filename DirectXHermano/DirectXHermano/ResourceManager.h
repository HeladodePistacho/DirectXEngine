#pragma once
#include <vector>
#include <queue>

class Render;

class Resource;
class ShaderProgram;
class Mesh;
class Submesh;

class aiScene;
class aiNode;
class aiMesh;

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager();

	void Start(Render& ren);
	void Update();

	Mesh& GetCube() const { return (*cube_mesh); }
	ShaderProgram& GetShader() const { return (*mesh_shader); }

	Mesh& DrawMeshesUI();

	void ImportResource(const char* path, Render& ren);

private:
	std::vector<Resource*> resources;

	//Test
	ShaderProgram* mesh_shader = nullptr;
	Mesh* cube_mesh = nullptr;

	void LoadShaders(Render& ren);
	void LoadCube(Render& ren);

	//Mesh load stuff
	void ImportMesh(const char* path, Render& ren);
	std::vector<Submesh*> ProcessNode(const aiScene* scene, aiNode* node, Render& ren);
	Submesh* ProcessMesh(const aiScene* scene, aiMesh* mesh, Render& ren);
	std::queue<aiNode*> nodes;
};