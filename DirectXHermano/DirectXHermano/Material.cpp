#include "Material.h"
#include "TextureResource.h"

Material::Material(std::string path) : Resource(RESOURCE_TYPE::MATERIAL, path)
{

}

void Material::SetAlbedoTexture(TextureResource * new_albedo)
{
	if (new_albedo)
		albedo_texture = new_albedo;
}

void Material::BindTexture(Render& ren, TEXTURE_TYPE type)
{
	switch (type)
	{
	case ALBEDO:
		if (albedo_texture)
			albedo_texture->Bind(ren);
		break;

	default:
		break;
	}
}
