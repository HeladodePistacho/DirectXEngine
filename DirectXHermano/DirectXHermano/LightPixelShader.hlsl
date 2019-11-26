struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

texture2D difuse_color_texture : register(t0);
texture2D normals_texture : register(t1);
texture2D position_texture : register(t2);

SamplerState samplerstate;

float ambient = 0.1f;

float4 main(VSOUT vertex_out) : SV_Target
{
	float4 final_color = difuse_color_texture.Sample(samplerstate, vertex_out.texture_coords) * ambient;

	return final_color;
}