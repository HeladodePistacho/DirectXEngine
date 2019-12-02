#include "Texture.h"

Texture::Texture(Render & ren, unsigned char* data, unsigned int width, unsigned int height, unsigned int color_channels)
{
	if (color_channels == 3) color_channels = 4;
	if (data == nullptr) is_empty = true;

	D3D11_TEXTURE2D_DESC texture_descriptor = {};

	texture_descriptor.Width = width;
	texture_descriptor.Height = height;
	texture_descriptor.MipLevels = 0;
	texture_descriptor.ArraySize = 1;
	texture_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture_descriptor.SampleDesc.Count = 1;
	texture_descriptor.SampleDesc.Quality = 0;
	texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
	texture_descriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture_descriptor.CPUAccessFlags = 0;
	texture_descriptor.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Usable without mip mapping
	//D3D11_SUBRESOURCE_DATA texture = {};
	//texture.pSysMem = data;
	//texture.SysMemPitch = width * color_channels * sizeof(unsigned char);

	ID3D11Texture2D* texture_pointer = nullptr;
	HRESULT result = GetDevice(ren)->CreateTexture2D(&texture_descriptor, nullptr, &texture_pointer);

	if (FAILED(result))
		return;

	//Add data to texture with mipmapping
	GetContext(ren)->UpdateSubresource(texture_pointer, 0u, nullptr, data, width * color_channels * sizeof(unsigned char), 0u);

	D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_descriptor = {};
	resource_view_descriptor.Format = texture_descriptor.Format;
	resource_view_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_view_descriptor.Texture2D.MipLevels = -1;
	resource_view_descriptor.Texture2D.MostDetailedMip = 0;

	result = GetDevice(ren)->CreateShaderResourceView(texture_pointer, &resource_view_descriptor, &texture_data);
	if (FAILED(result))
		return;

	GetContext(ren)->GenerateMips(texture_data);
}

Texture::~Texture()
{
	if (texture_data)
		delete texture_data;

	
}

void Texture::Bind(Render & ren)
{
	GetContext(ren)->PSSetShaderResources(0u, 1u, &texture_data);
}
