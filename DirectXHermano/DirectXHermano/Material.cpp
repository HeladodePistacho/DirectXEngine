#include "Material.h"
#include "TextureResource.h"

Material::Material(std::string path) : Resource(RESOURCE_TYPE::MATERIAL, path)
{
}

Material::~Material()
{
	if (colors != nullptr)
		delete colors;

	if (material_buffer != nullptr)
		delete material_buffer;
}

void Material::SetAlbedoTexture(TextureResource * new_albedo)
{
	if (new_albedo)
	{
		albedo_texture = new_albedo;

		Texture* tmp = albedo_texture->GetTextureBindable();
		if (tmp == nullptr || tmp->IsNull())
		{
			material_struct.use_only_color = 1.0f;
			needs_update = true;
		}
		else
		{
			if (material_struct.use_only_color > 0.0)
			{
				material_struct.use_only_color = -1.0f;
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

void Material::SetNormalTexture(TextureResource * new_normals)
{
	normal_texture = new_normals;
}

void Material::InitColorBuffer(Render & ren)
{
	albedo_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	material_struct = { -1.0f, 2.0f };

	if (colors == nullptr)
		colors = new ConstBuffer<Color>(ren, albedo_color, BUFFER_TYPE::PIXEL_SHADER_BUFFER);

	if(material_buffer == nullptr)
		material_buffer = new ConstBuffer<MaterialStructBuffer>(ren, material_struct, BUFFER_TYPE::PIXEL_SHADER_BUFFER);
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

	case NORMAL:
		if (normal_texture != nullptr)
			normal_texture->Bind(ren);
		break;

	default:
		break;
	}

	if(colors != nullptr)
		colors->Bind(ren);

	if (material_buffer != nullptr)
		material_buffer->BindSlot(ren, 1u);
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
	material_buffer->Update(ren, material_struct);
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
		if (normal_texture)
			return normal_texture->GetTexture();

		break;
	case HEIGHT:
		break;
	}
	return nullptr;
}

void Material::SetSpecular(int spec)
{
	material_struct.specular_value = (float)spec;
	needs_update = true;
}
