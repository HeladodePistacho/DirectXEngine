
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
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

float4 main(VSOUT vertex_out) : SV_Target
{
	if (only_color.x > 0.0f)
		return albedo_color;
	else return (albedo.Sample(samplerstate, vertex_out.texture_coords) * albedo_color);

	//return float4(vertex_out.texture_coords.x, vertex_out.texture_coords.y, 0.0f, 1.0f);
	//return float4(vertex_out.color, 1.0f);
	//return float4(albedo_color);
}