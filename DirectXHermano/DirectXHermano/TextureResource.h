#pragma once
#include "Resource.h"

class Texture;

class TextureResource : Resource
{
public:
	TextureResource(std::string path);
	~TextureResource();

private:
	Texture* texture = nullptr;
};