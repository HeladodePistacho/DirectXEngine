
struct VSOUT
{
	float4 out_position : SV_Position;
	float3 normal : Normale;
	float3 position : Position;
	float2 texture_coords : Texcoord0;
};

struct PSOUT
{
	float4 diffuse_color : SV_Target0;
	float4 normals : SV_Target1;
	float4 position : SV_Target2;
	float4 specular : SV_Target3;
};

Texture2D albedo : register(t0);
SamplerState samplerstate;

cbuffer texture_colors : register(b0)
{
	float4 albedo_color;
};

cbuffer material_buffer : register(b1)
{
	float only_color;
	float specular_value;
	float metal_value;
};

PSOUT main(VSOUT vertex_out)
{
	PSOUT pixel_shader_out;

	if (only_color.x > 0.0f)
		pixel_shader_out.diffuse_color = albedo_color;
	else pixel_shader_out.diffuse_color = (albedo.Sample(samplerstate, vertex_out.texture_coords) * albedo_color);

	pixel_shader_out.normals = float4(vertex_out.normal, 1.0f);
	pixel_shader_out.position = float4(vertex_out.position, 1.0f);
	pixel_shader_out.specular = float4(specular_value, metal_value, 0.0f, 1.0f);

	return pixel_shader_out;
}