#pragma once
#include <vector>
#include <queue>
#include <map>
#include "Resource.h"

class Render;

class ShaderProgram;
class Mesh;
class Submesh;
class Material;
class Texture;
class File;

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
	Material& DrawMaterialUI();

	void ImportResource(const File* file, Render& ren);

private:
	std::multimap<RESOURCE_TYPE, Resource*> mapped_resources;

	//Test
	ShaderProgram* mesh_shader = nullptr;
	Mesh* cube_mesh = nullptr;

	void LoadShaders(Render& ren);
	void LoadCube(Render& ren);

	//Import stuff
	const char* actual_resource_path = nullptr;

	//Mesh load stuff
	void ImportMesh(const char* path, Render& ren);
	std::vector<Submesh*> ProcessNode(const aiScene* scene, aiNode* node, Render& ren);
	Submesh* ProcessMesh(const aiScene* scene, aiMesh* mesh, Render& ren);
	std::queue<aiNode*> nodes;

	//Image Load Sruff
	Texture* ImportImage(const char* path, Render& ren);
	
};