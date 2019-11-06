#include "Camera.h"
#include "ImGui\imgui.h"
#include <math.h>

//-DirectX::XM_PI/18.0f
//DirectX::XM_PI / 2.0f

Camera::Camera(float width, float height) : yaw(0.0f), pitch(DirectX::XM_PI), roll(0.0f), fov((1.0f/2.0f)*DirectX::XM_PI), position(0.0f, 0.0f, -5.0f), z_near(0.5f), z_far(50.0f), aspect_ratio(width / height) {}

Camera::Camera(const Camera & cam)
{
	yaw = cam.yaw;
	pitch = cam.pitch;
	roll = cam.roll;
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

void Camera::DrawUI()
{
	bool open = true;
	ImGui::Begin("Camera Options", &open, ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	ImGui::Text("Yaw: %.2f, Pitch: %.2f, Roll: %.2f", DirectX::XMConvertToDegrees(yaw), DirectX::XMConvertToDegrees(pitch), DirectX::XMConvertToDegrees(roll));

	if (ImGui::SliderFloat("Z_Near", &z_near, 0.05f, 1.0f, "%.1f"))
		needs_update = true;

	if (ImGui::SliderFloat("Z_Far", &z_far, 10.0f, 100.0f, "%.1f"))
		needs_update = true;

	if (ImGui::SliderFloat("Field Of View", &fov, (2.0f / 9.0f) * DirectX::XM_PI, (2.0f / 3.0f) * DirectX::XM_PI, "%.1f"))
		needs_update = true;

	ImGui::End();
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
	position.x += x * 0.5f;
	position.y += y * 0.5f;
	position.z += z * 0.5f;

	needs_update = true;
}

void Camera::Rotate(int y_desplace, int x_displace)
{
	// = 0.0f;


     	yaw += (y_desplace * 0.005f);
	if (abs(yaw) > (0.5f * DirectX::XM_PI))
	{
		if (yaw > 0.0f)
			yaw = (0.5f * DirectX::XM_PI);
		else yaw = -(0.5f * DirectX::XM_PI); 
	}
	
	pitch += (x_displace * 0.005f);	
	if (abs(pitch) > (2 * DirectX::XM_PI))
	{
		pitch = 0.0f;
	}


	needs_update = true;
}

void Camera::ResetRotation()
{
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
}
