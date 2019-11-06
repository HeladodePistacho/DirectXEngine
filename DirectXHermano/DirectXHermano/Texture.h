#pragma once

#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Render& ren, unsigned char* data, unsigned int width, unsigned int height, unsigned int color_channels);
	~Texture();

	void Bind(Render& ren) override;

private:
	ID3D11ShaderResourceView* texture_data = nullptr;
};
