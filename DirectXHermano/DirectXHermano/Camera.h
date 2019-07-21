#pragma once
#include <DirectXMath.h>
#include "ConstBuffers.h"

class Camera
{
public:
	Camera(float width, float height);
	Camera(const Camera& cam);
	~Camera();

	Camera& operator=(const Camera& cam);

	void DrawUI();
	void Move(DirectX::XMFLOAT3);
	void Move(float, float, float);
	void Rotate(int, int);
	void ResetRotation();

	float yaw, pitch, roll; //in radians
	float fov; //in radians
	float aspect_ratio;
	float z_near, z_far;
	DirectX::XMFLOAT3 position;

	bool needs_update = false;
};
