#include "TextureResource.h"

TextureResource::TextureResource(std::string path) : Resource(RESOURCE_TYPE::TEXTURE, path)
{

}

TextureResource::~TextureResource()
{
	if (texture)
		delete texture;
}
