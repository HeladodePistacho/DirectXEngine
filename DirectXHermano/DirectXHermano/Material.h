#pragma once
#include "Resource.h"
#include "ConstBuffers.h"
#include <vector>

class TextureResource;
class ID3D11ShaderResourceView;
class Render;

struct Color
{
	float components[4];
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

	float* GetColor(TEXTURE_TYPE);
	void UpdateColor(TEXTURE_TYPE, Render&);

	//test
	ID3D11ShaderResourceView* GetTexture(TEXTURE_TYPE) const;

private:
	TextureResource* albedo_texture = nullptr;
	Color albedo_color;

	ConstBuffer<Color>* colors = nullptr;
};
