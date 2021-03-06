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

	void DrawMaterialEditorUI(Render& ren);

	void ImportResource(const File* file, Render& ren);

	//Resources Stuff
	Resource* GetResourceByPath(const char* full_path, RESOURCE_TYPE type);
	Resource* GetResourceByName(const char* name, RESOURCE_TYPE type);
	Resource* DrawResourceSelectableUI(RESOURCE_TYPE type);

	Material* GetMaterialByName(std::string name) const;
	void SetMaterialToModify(Material* current_mat);

	//Test for screeen
	ShaderProgram* screen_shader = nullptr;
	Mesh* screen_mesh = nullptr;
	Mesh* sphere_mesh = nullptr;
	Mesh* light_mesh = nullptr;

	ShaderProgram* light_shader = nullptr;
	ShaderProgram* grid_shader = nullptr;
	ShaderProgram* light_mesh_shader = nullptr;

private:
	std::multimap<RESOURCE_TYPE, Resource*> mapped_resources;

	//Test
	ShaderProgram* mesh_shader = nullptr;
	

	Mesh* cube_mesh = nullptr;

	void LoadShaders(Render& ren);
	void LoadCube(Render& ren);
	void LoadNullTexture(Render& ren);
	void LoadPlane(Render& ren);
	void LoadDefaultMaterial(Render& ren);

	//Import stuff
	const char* actual_resource_path = nullptr;
	char my_path[256];

	//Mesh load stuff
	void ImportMesh(const char* path, Render& ren);
	std::vector<Submesh*> ProcessNode(const aiScene* scene, aiNode* node, Render& ren);
	Submesh* ProcessMesh(const aiScene* scene, aiMesh* mesh, Render& ren);
	std::queue<aiNode*> nodes;

	//Image Load Sruff
	void ImportTexture(const char* path, Render& ren);
	Texture* ImportImage(const char* path, Render& ren);
	const char* ProcessMaterials(const aiScene* scene, aiNode* node, Render& ren);

	//Material editor
	Material* material_to_modify = nullptr;
	char material_name_buffer[128];

};