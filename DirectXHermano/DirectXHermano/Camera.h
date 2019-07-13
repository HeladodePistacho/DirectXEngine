#pragma once
#include <DirectXMath.h>
#include "ConstBuffers.h"

struct transform
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX perspective;
};

class Camera
{
public:
	Camera(Render& ren, float width, float height);
	~Camera();

	void BindAll(Render& ren);
	void BindViewMatrix(Render& ren);
	void BindProjectionMatrix(Render& ren);


private:
	float yaw, pitch;
	float fov; //in radians
	float z_near, z_far;
	DirectX::XMFLOAT3 position;

	ConstBuffer<DirectX::XMMATRIX>* world_matrix = nullptr;
	ConstBuffer<DirectX::XMMATRIX>* view_matrix = nullptr;
	ConstBuffer<DirectX::XMMATRIX>* perspective_matrix = nullptr;

	ConstBuffer<transform>* trans_bufer = nullptr;
};
