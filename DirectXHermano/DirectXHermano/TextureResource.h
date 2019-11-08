#pragma once
#include "Resource.h"
#include "Texture.h"

class Sampler;

class TextureResource : public Resource
{
public:
	TextureResource(std::string path);
	~TextureResource();

	void AddTexture(Texture* new_tex);
	void AddSampler(Sampler* new_sampler);
	void Bind(Render& ren);

	ID3D11ShaderResourceView* GetTexture();

private:
	Texture* texture = nullptr;
	Sampler* texture_sampler = nullptr;
};