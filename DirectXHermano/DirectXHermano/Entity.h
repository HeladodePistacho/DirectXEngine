#pragma once
#include <vector>
#include <DirectXMath.h>
#include "ConstBuffers.h"

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

class Entity
{
public:
	Entity() = default;
	~Entity();

	void Update();
	void Draw(Render& ren);

	bool IsDeleted() { return needs_delete; }
	void Delete();
private:
	bool needs_delete = false;

	Transform* transform = nullptr;

};