
struct VSOUT
{
	float3 normal : Normal;
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

struct PSOUT
{
	float4 diffuse_color : SV_Target0;
	float4 normals : SV_Target1;
};

Texture2D albedo : register(t0);
SamplerState samplerstate;

cbuffer texture_colors : register(b0)
{
	float4 albedo_color;
};

cbuffer test : register(b1)
{
	float4 only_color;
};

PSOUT main(VSOUT vertex_out)
{
	PSOUT pixel_shader_out;


	if (only_color.x > 0.0f)
		pixel_shader_out.diffuse_color = albedo_color;
	else pixel_shader_out.diffuse_color = (albedo.Sample(samplerstate, vertex_out.texture_coords) * albedo_color);

	pixel_shader_out.normals = float4(vertex_out.normal, 1.0f);

	return pixel_shader_out;
	//return float4(vertex_out.texture_coords.x, vertex_out.texture_coords.y, 0.0f, 1.0f);
	//return float4(vertex_out.color, 1.0f);
	//return float4(albedo_color);
}