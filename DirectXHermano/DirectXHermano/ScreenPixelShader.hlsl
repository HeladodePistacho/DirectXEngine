
struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

texture2D final_color_texture;
SamplerState samplerstate;

float4 main(VSOUT vertex_out) : SV_Target
{
	float4 color = final_color_texture.Sample(samplerstate, vertex_out.texture_coords);
	return color;
}