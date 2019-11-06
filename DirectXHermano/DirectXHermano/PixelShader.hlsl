
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

Texture2D text;
SamplerState samplerstate;

float4 main(VSOUT vertex_out) : SV_Target
{
	return text.Sample(samplerstate, vertex_out.texture_coords);
	//return float4(vertex_out.texture_coords.x, vertex_out.texture_coords.y, 0.0f, 1.0f);
	//return float4(vertex_out.color, 1.0f);
}