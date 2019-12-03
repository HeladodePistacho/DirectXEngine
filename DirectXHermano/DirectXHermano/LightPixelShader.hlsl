struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

//texture2D difuse_color_texture : register(t0);
//texture2D normals_texture : register(t1);
//texture2D position_texture : register(t2);
//
//SamplerState samplerstate;

float ambient = 0.1f;

cbuffer light_buffer : register(b0)
{
	float4 color;
	float4 position;
	float4 direction;
};

float4 main(VSOUT vertex_out) : SV_Target
{
	if(color.x < 1.0f)
		return float4(1.0f, 0.0f, 1.0f, 1.0f);
	else return float4(0.0f, 0.0f, 1.0f, 1.0f);
}