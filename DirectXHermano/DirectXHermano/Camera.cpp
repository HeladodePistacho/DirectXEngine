#include "Camera.h"

//-DirectX::XM_PI/18.0f
//DirectX::XM_PI / 2.0f

Camera::Camera(float width, float height) : yaw(0), pitch(0), fov((1.0f/2.0f)*DirectX::XM_PI), position(0.0f, 0.0f, 5.0f), z_near(0.5f), z_far(25.0f), aspect_ratio(width / height) {}

Camera::Camera(const Camera & cam)
{
	yaw = cam.yaw;
	pitch = cam.pitch;
	fov = cam.fov;
	position = cam.position;
	z_near = cam.z_near;
	z_far = cam.z_far;
	aspect_ratio = cam.aspect_ratio;
}

Camera::~Camera()
{
	
}

Camera& Camera::operator=(const Camera & cam)
{
	Camera ret(cam);
	return ret;
}

void Camera::Move(DirectX::XMFLOAT3 movement)
{
	position.x += movement.x;
	position.y += movement.y;
	position.z += movement.z;

	needs_update = true;
}

void Camera::Move(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

	needs_update = true;
}
