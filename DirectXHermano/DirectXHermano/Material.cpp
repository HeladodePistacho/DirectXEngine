#include "Material.h"
#include "TextureResource.h"

Material::Material(std::string path) : Resource(RESOURCE_TYPE::MATERIAL, path)
{
}

Material::~Material()
{
	if (colors != nullptr)
		delete colors;

	if (use_colors != nullptr)
		delete colors;
}

void Material::SetAlbedoTexture(TextureResource * new_albedo)
{
	if (new_albedo)
	{
		albedo_texture = new_albedo;

		Texture* tmp = albedo_texture->GetTextureBindable();
		if (tmp == nullptr || tmp->IsNull())
		{
			use_only_colors.components[0] = 1.0f;
			needs_update = true;
		}
		else
		{
			if (use_only_colors.components[0] > 0.0)
			{
				use_only_colors.components[0] = -1.0f;
				needs_update = true;
			}
		}
	}
}

void Material::SetAlbedoColor(Render& ren, float r, float g, float b, float a)
{
	albedo_color = { r, g, b, a };

	colors->Update(ren, albedo_color);
}

void Material::InitColorBuffer(Render & ren)
{
	albedo_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	use_only_colors = { -1.0f, -1.0f, -1.0f, -1.0f };

	if (colors == nullptr)
		colors = new ConstBuffer<Color>(ren, albedo_color, BUFFER_TYPE::PIXEL_SHADER_BUFFER);

	if(use_colors == nullptr)
		use_colors = new ConstBuffer<Color>(ren, use_only_colors, BUFFER_TYPE::PIXEL_SHADER_BUFFER);
}

void Material::BindTexture(Render& ren, TEXTURE_TYPE type)
{
	if (needs_update)
		Update(ren);

	switch (type)
	{
	case ALBEDO:
		if (albedo_texture != nullptr)
			albedo_texture->Bind(ren);
		break;

	default:
		break;
	}

	if(colors != nullptr)
		colors->Bind(ren);

	if (use_colors != nullptr)
		use_colors->BindSlot(ren, 1u);
}

float* Material::GetColor(TEXTURE_TYPE color_type)
{
	switch (color_type)
	{
	case ALBEDO:
		return albedo_color.components;
		break;
	case NORMAL:
		break;
	case HEIGHT:
		break;
	}
}

void Material::Update(Render & ren)
{
	UpdateColorUsage(ren);

	needs_update = false;
}

void Material::UpdateColorUsage(Render & ren)
{
	use_colors->Update(ren, use_only_colors);
}

void Material::UpdateColor(TEXTURE_TYPE color_type, Render& ren)
{
	switch (color_type)
	{
	case ALBEDO:
		colors->Update(ren, albedo_color);
		break;
	case NORMAL:
		break;
	case HEIGHT:
		break;
	}
}

ID3D11ShaderResourceView* Material::GetTexture(TEXTURE_TYPE texture_type) const
{
	switch (texture_type)
	{
	case ALBEDO:
		if(albedo_texture)
			return albedo_texture->GetTexture();
		break;
	case NORMAL:
		break;
	case HEIGHT:
		break;
	}
	return nullptr;
}
