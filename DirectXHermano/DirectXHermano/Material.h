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

struct MaterialStructBuffer
{
	float use_only_color;
	float specular_value;
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
	~Material();

	void SetAlbedoTexture(TextureResource* new_albedo);
	void SetAlbedoColor(Render& ren, float r, float g, float b, float a = 1.0f);

	void SetNormalTexture(TextureResource* new_normals);

	void InitColorBuffer(Render& ren);

	void BindTexture(Render& ren, TEXTURE_TYPE);

	//Color
	float* GetColor(TEXTURE_TYPE);
	void UpdateColor(TEXTURE_TYPE, Render&);

	//Textures
	ID3D11ShaderResourceView* GetTexture(TEXTURE_TYPE) const;

	//Speculatr
	float GetSpecular() const { return material_struct.specular_value; }
	void SetSpecular(int spec);

	void SetNeedsUpdate(bool state) { needs_update = state; }
private:
	TextureResource* albedo_texture = nullptr;
	Color albedo_color;

	TextureResource* normal_texture = nullptr;

	//This holds the values to use or not the textures
	MaterialStructBuffer material_struct;

	ConstBuffer<Color>* colors = nullptr;
	ConstBuffer<MaterialStructBuffer>* material_buffer = nullptr;

	//Update Elements
	bool needs_update = false;
	void Update(Render& ren);
	void UpdateColorUsage(Render& ren);

};
