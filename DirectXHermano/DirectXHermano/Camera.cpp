#include "Camera.h"
#include "ImGui\imgui.h"
#include <math.h>

//-DirectX::XM_PI/18.0f
//DirectX::XM_PI / 2.0f

Camera::Camera(float width, float height) : yaw(0.0f), pitch(DirectX::XM_PI), roll(0.0f), fov((1.0f/2.0f)*DirectX::XM_PI), z_near(0.5f), z_far(200.0f), aspect_ratio(width / height) 
{
	camera_struct.cam_position = { 0.0f, 5.0f, 8.0f };
}

Camera::Camera(const Camera & cam)
{
	yaw = cam.yaw;
	pitch = cam.pitch;
	roll = cam.roll;
	fov = cam.fov;
	camera_struct.cam_position = cam.camera_struct.cam_position;
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
	ImGui::Begin("Camera Options", &open);

	ImGui::Text("Yaw: %.2f, Pitch: %.2f, Roll: %.2f", DirectX::XMConvertToDegrees(yaw), DirectX::XMConvertToDegrees(pitch), DirectX::XMConvertToDegrees(roll));

	if (ImGui::SliderFloat("Z_Near", &z_near, 0.05f, 1.0f, "%.1f"))
		needs_update = true;

	if (ImGui::SliderFloat("Z_Far", &z_far, 10.0f, 300.0f, "%.1f"))
		needs_update = true;

	if (ImGui::SliderFloat("Field Of View", &fov, (2.0f / 9.0f) * DirectX::XM_PI, (2.0f / 3.0f) * DirectX::XM_PI, "%.1f"))
		needs_update = true;

	ImGui::SliderFloat("Camera Speed", &move_speed, 1.0f, 200.0f, "%.1f");

	ImGui::End();
}

void Camera::Move(DirectX::XMFLOAT3 movement)
{
	camera_struct.cam_position.x += movement.x;
	camera_struct.cam_position.y += movement.y;
	camera_struct.cam_position.z += movement.z;

	needs_update = true;
}

void Camera::Move(float x, float y, float z, float dt)
{
	camera_struct.cam_position.x += x * move_speed * dt;
	camera_struct.cam_position.y += y * move_speed * dt;
	camera_struct.cam_position.z += z * move_speed * dt;

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

void Camera::UpdateCameraBuffer(Render& ren)
{
	if (!camera_buffer)
		camera_buffer = new ConstBuffer<CameraBuffer>(ren, camera_struct, BUFFER_TYPE::PIXEL_SHADER_BUFFER);
	else camera_buffer->Update(ren, camera_struct);
}

void Camera::UpdateCameraMatrixBuffer(Render & ren)
{

	DirectX::XMMATRIX tmp_0 = (DirectX::XMMatrixRotationRollPitchYaw(yaw, pitch, roll)) * DirectX::XMMatrixTranslation(camera_struct.cam_position.x, camera_struct.cam_position.y, camera_struct.cam_position.z);
	DirectX::XMMATRIX tmp4 = DirectX::XMMatrixTranspose(tmp_0);

	DirectX::XMStoreFloat4x4(&camera_matrixs_struct.view_matrix, tmp4);
	DirectX::XMStoreFloat4x4(&camera_matrixs_struct.perspective_matrix, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far))));

	if (!camera_matrixs_buffer)
		camera_matrixs_buffer = new ConstBuffer<CameraMatrixBuffer>(ren, camera_matrixs_struct, BUFFER_TYPE::PIXEL_SHADER_BUFFER);
	else camera_matrixs_buffer->Update(ren, camera_matrixs_struct);
}
