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
	Entity();
	~Entity();

	void Update();
	void Draw(Render& ren);

	bool IsDeleted() { return needs_delete; }
	void Delete();

	MeshRenderer& GetMeshRenderer() const { return (*mesh_render); }
private:
	bool needs_delete = false;

	Transform* transform = nullptr;
	MeshRenderer* mesh_render = nullptr;

};