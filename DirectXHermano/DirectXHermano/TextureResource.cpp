#include "TextureResource.h"
#include "Sampler.h"

TextureResource::TextureResource(std::string path) : Resource(RESOURCE_TYPE::TEXTURE, path)
{

}

TextureResource::~TextureResource()
{
	if (texture)
		delete texture;
}

void TextureResource::AddTexture(Texture* new_tex)
{
	if (new_tex)
		texture = new_tex;
}

void TextureResource::AddSampler(Sampler* new_sampler)
{
	if (new_sampler)
		texture_sampler = new_sampler;
}

void TextureResource::Bind(Render & ren)
{
	if(texture)
		texture->Bind(ren);

	if (texture_sampler)
		texture_sampler->Bind(ren);
}

ID3D11ShaderResourceView* TextureResource::GetTexture()
{
	return texture->GetTexture();
}
