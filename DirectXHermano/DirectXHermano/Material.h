#pragma once
#include "Resource.h"
#include <vector>

class TextureResource;

struct Color
{
	float r, g, b, a;
};

class Material : Resource
{
public:
	Material(std::string path);
	~Material();

	void SetAlbedoTexture(TextureResource* new_albedo);
	void SetAlbedoColor(float r, float g, float b, float a = 1.0f);

private:
	TextureResource* AlbedoTexture = nullptr;
	Color AlbedoColor;
};
