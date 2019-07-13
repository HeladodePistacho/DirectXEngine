#include "Camera.h"



Camera::Camera(Render& ren, float width, float height) : yaw(0), pitch(0), fov((1.0f/2.0f)*DirectX::XM_PI), position(0.0f, 1.0f, 0.0f), z_near(0.5f), z_far(25.0f)
{
	//Set the buffers
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
	);
	world_matrix = new ConstBuffer<DirectX::XMMATRIX>(ren, world, BUFFER_TYPE::VERTEX_SHADER_BUFFER);

	DirectX::XMMATRIX view = DirectX::XMMatrixTranspose(world);
	view_matrix = new ConstBuffer<DirectX::XMMATRIX>(ren, view, BUFFER_TYPE::VERTEX_SHADER_BUFFER);

	DirectX::XMMATRIX perspective = DirectX::XMMatrixPerspectiveFovLH(fov, width / height, z_near, z_far);
	perspective_matrix = new ConstBuffer<DirectX::XMMATRIX>(ren, perspective, BUFFER_TYPE::VERTEX_SHADER_BUFFER);

	transform trans = 
	{
		{ DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z))},
		{ DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(fov, width / height, z_near, z_far)) }
	};

	trans_bufer = new ConstBuffer<transform>(ren, trans, BUFFER_TYPE::VERTEX_SHADER_BUFFER);
}

Camera::~Camera()
{
	if (world_matrix)
	{
		delete world_matrix;
		world_matrix = nullptr;
	}

	if (view_matrix)
	{
		delete view_matrix;
		view_matrix = nullptr;
	}

	if (perspective_matrix)
	{
		delete perspective_matrix;
		perspective_matrix = nullptr;
	}
}

void Camera::BindAll(Render & ren)
{
	trans_bufer->Bind(ren);
	//BindViewMatrix(ren);
	//BindProjectionMatrix(ren);
}

void Camera::BindViewMatrix(Render & ren)
{
	view_matrix->Bind(ren);
}

void Camera::BindProjectionMatrix(Render & ren)
{
	perspective_matrix->Bind(ren);
}

