#pragma once
#include "Resource.h"
#include "ConstBuffers.h"
#include <vector>

class TextureResource;
class Render;

struct Color
{
	float r, g, b, a;
};

enum TEXTURE_TYPE
{
	ALBEDO = 0,
	NORMAL,
	HEIGHT
};

class Material : public Resource
{
public:
	Material(std::string path);
	//~Material();

	void SetAlbedoTexture(TextureResource* new_albedo);
	void SetAlbedoColor(Render& ren, float r, float g, float b, float a = 1.0f);

	void InitColorBuffer(Render& ren);

	void BindTexture(Render& ren, TEXTURE_TYPE);
private:
	TextureResource* albedo_texture = nullptr;
	Color albedo_color;

	ConstBuffer<Color>* colors = nullptr;
};
