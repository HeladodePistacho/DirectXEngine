#pragma once

#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Render& ren, unsigned char* data, unsigned int width, unsigned int height, unsigned int color_channels);
	~Texture();

	void Bind(Render& ren) override;

	ID3D11ShaderResourceView* GetTexture() const { return texture_data; }

private:
	ID3D11ShaderResourceView* texture_data = nullptr;
};
