#pragma once
#include <vector>
#include <DirectXMath.h>
#include "ConstBuffers.h"

class MeshRenderer;

class Transform
{
public:
	Transform();
	~Transform() = default;

	void Update();
	void BuildMatrix();

	void DrawTransformUI();

	bool NeedsUpdate() const { return needs_update; }
	DirectX::XMMATRIX GetMatrix() { needs_update = false;  return model_matrix; }
private:
	DirectX::XMMATRIX model_matrix;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation_euler;
	DirectX::XMFLOAT3 scale;

	DirectX::XMVECTOR rotation_quaternion;

	bool needs_update = false;
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

	Transform* GetTransform() { return transform; }
private:
	std::string name;

	bool needs_delete = false;
	bool is_selected = false;

	Transform* transform = nullptr;
	MeshRenderer* mesh_render = nullptr;

};