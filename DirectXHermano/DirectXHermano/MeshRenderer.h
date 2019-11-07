#pragma once
#include "ConstBuffers.h"
#include <DirectXMath.h>
#include <vector>

class Mesh;
class Entity;
class ResourceManager;
class Material;

class MeshRenderer
{
public:
	struct matrix_buffers
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	MeshRenderer(Entity* _parent);
	~MeshRenderer();

	void Draw(Render& ren);

	void PrepareMatrices(Render& ren);
	void SetMesh(const Mesh* mesh) { mesh_to_draw = (Mesh*)mesh; }

	void DrawMeshRendererUI(ResourceManager& res_manager);

	void AddMaterial(Material* new_mat);
private:
	Entity* parent = nullptr;

	ConstBuffer<matrix_buffers>* matrices = nullptr;
	Mesh* mesh_to_draw = nullptr;
	std::vector<Material*> meshes_materials;
};