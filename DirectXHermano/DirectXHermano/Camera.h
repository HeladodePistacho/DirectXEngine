#pragma once
#include <DirectXMath.h>
#include "ConstBuffers.h"

struct CameraBuffer
{
	DirectX::XMFLOAT3 cam_position;
};

class Camera
{
public:
	Camera(float width, float height);
	Camera(const Camera& cam);
	~Camera();

	Camera& operator=(const Camera& cam);

	void DrawUI();
	void Move(DirectX::XMFLOAT3);
	void Move(float, float, float, float);
	void Rotate(int, int);
	void ResetRotation();

	float yaw, pitch, roll; //in radians
	float fov; //in radians
	float aspect_ratio;
	float z_near, z_far;

	bool needs_update = false;

	float move_speed = 15.0f;

	//Render stuff
	CameraBuffer camera_struct;
	ConstBuffer<CameraBuffer>* camera_buffer = nullptr;
	void UpdateBuffer(Render& ren);
};
