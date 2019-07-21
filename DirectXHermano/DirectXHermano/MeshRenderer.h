#pragma once
#include "ConstBuffers.h"
#include <DirectXMath.h>

class Mesh;
class Entity;

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

private:
	Entity* parent = nullptr;

	ConstBuffer<matrix_buffers>* matrices = nullptr;
	Mesh* mesh_to_draw = nullptr;
};