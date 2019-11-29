#pragma once
#include "ConstBuffers.h"

enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT
};

class LightComponent
{
public:
	LightComponent(LIGHT_TYPE);

	void Draw(Render& ren);

	//UI stuff
	void DrawUI();
	
private:
	LIGHT_TYPE light_type;
	float light_color[3];

	ConstBuffer<float>* light_buffer;

	//UI stuff
	const char* LightTypeToChar();
};