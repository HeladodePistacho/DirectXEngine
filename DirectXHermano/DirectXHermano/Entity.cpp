#include "Entity.h"

//----------------------------------- TRANSFORM ------------------------------------------------
Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation_euler(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation_quaternion(DirectX::XMQuaternionRotationRollPitchYaw(rotation_euler.x, rotation_euler.y, rotation_euler.z))
{
	BuildMatrix();
}

void Transform::Update()
{
	if (needs_update)
	{
		BuildMatrix();
	}
}

void Transform::BuildMatrix()
{
	model_matrix = DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationQuaternion(rotation_quaternion) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixTranslation(position.x, position.y, position.z)
	);
}

//----------------------------------- TRANSFORM ------------------------------------------------
//-----------------------------------  ENTITY   -------------------------------------------------

Entity::~Entity()
{
	if (transform)
	{
		delete transform;
		transform = nullptr;
	}
}

void Entity::Update()
{
}

void Entity::Draw(Render & ren)
{
}

void Entity::Delete()
{
	needs_delete = true;
}
