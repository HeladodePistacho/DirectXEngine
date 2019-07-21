#pragma once
#include <vector>
#include <DirectXMath.h>
#include "ConstBuffers.h"

class Mesh;

class Transform
{
public:
	Transform();
	~Transform() = default;

	void Update();
	void BuildMatrix();

private:
	DirectX::XMMATRIX model_matrix;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation_euler;
	DirectX::XMFLOAT3 scale;

	DirectX::XMVECTOR rotation_quaternion;

	bool needs_update = false;
};

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
	MeshRenderer();
	~MeshRenderer();

	void Draw(Render& ren);
	
	void PrepareMatrices(Render& ren);
	void SetMesh(const Mesh* mesh) { mesh_to_draw = (Mesh*)mesh; }
private:
	ConstBuffer<matrix_buffers>* matrices = nullptr;
	Mesh* mesh_to_draw = nullptr;
};

class Entity
{
public:
	Entity(int id);
	~Entity();

	void Update();
	void Draw(Render& ren);

	bool IsDeleted() { return needs_delete; }
	void Delete();

	MeshRenderer& GetMeshRenderer() const { return (*mesh_render); }

	const char* GetName() const { return name.c_str(); }

	bool IsSelected() const { return is_selected; }
	void SetSelected(bool selected) { is_selected = selected; }

	void DrawUI();
private:
	std::string name;

	bool needs_delete = false;
	bool is_selected = false;

	Transform* transform = nullptr;
	MeshRenderer* mesh_render = nullptr;

};