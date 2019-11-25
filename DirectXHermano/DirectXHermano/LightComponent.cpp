#include "LightComponent.h"

LightComponent::LightComponent(LIGHT_TYPE type) : light_type(type)
{
	light_color[0] = 1.0f;
	light_color[1] = 1.0f;
	light_color[2] = 1.0f;
}
