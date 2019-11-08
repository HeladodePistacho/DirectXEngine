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
class TextureResource;
class Preset;
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

	const Mesh* DrawMeshesUI() const;
	const Material* DrawMaterialUI() const;
	const Preset* DrawPresetsUI()const;
	const TextureResource* DrawTextureUI() const;

	void DrawMaterialEditorUI(Render& ren);

	void ImportResource(const File* file, Render& ren);

	Material* GetMaterialByName(std::string name) const;
	void SetMaterialToModify(Material* current_mat);

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
	const char* ProcessMaterials(const aiScene* scene, aiNode* node, Render& ren);
	TextureResource* GetTextureByName(const char* full_path);
	Resource* GetResourceByPath(const char* full_path, RESOURCE_TYPE type);

	//Material editor
	Material* material_to_modify = nullptr;
	
};