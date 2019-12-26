#pragma once
#include "ConstBuffers.h"
#include <DirectXMath.h>

enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT
};

struct LightBuffer
{
	DirectX::XMFLOAT3 color;
	float type;

	DirectX::XMFLOAT3 position;
	float intensity;

	DirectX::XMFLOAT3 direction;
	float scale;
};

class LightComponent
{
public:
	LightComponent(LIGHT_TYPE);

	void Draw(Render& ren);
	void UpdateLightBuffer(Render& ren);

	//Setters
	void SetDirection(DirectX::XMFLOAT3 dir);
	void SetPosition(DirectX::XMFLOAT3 pos);

	//Getters
	int GetLightType() const { return (int)buffer_struct.type; }

	bool NeedsUpdate() const { return needs_update; }

	//UI stuff
	void DrawUI();
	
private:
	LIGHT_TYPE light_type;

	LightBuffer buffer_struct;
	ConstBuffer<LightBuffer>* light_buffer = nullptr;

	bool needs_update = false;

	//UI stuff
	const char* LightTypeToChar();
};