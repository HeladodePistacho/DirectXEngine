#pragma once
#include <DirectXMath.h>
#include "ConstBuffers.h"

struct CameraBuffer
{
	DirectX::XMFLOAT3 cam_position;
};

struct CameraMatrixBuffer
{
	DirectX::XMFLOAT4X4 view_matrix;
	DirectX::XMFLOAT4X4 perspective_matrix;
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
	CameraMatrixBuffer camera_matrixs_struct;

	ConstBuffer<CameraBuffer>* camera_buffer = nullptr;
	ConstBuffer<CameraMatrixBuffer>* camera_matrixs_buffer = nullptr;

	void UpdateCameraBuffer(Render& ren);
	void UpdateCameraMatrixBuffer(Render& ren);
};
