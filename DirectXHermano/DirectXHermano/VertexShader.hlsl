
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
};

cbuffer constant_buffer
{
	matrix tranform;
};

VSOUT main(float2 position : Position, float3 color : Color)
{
	VSOUT vertex_out;
	vertex_out.position = mul(float4(position.x, position.y, 0.0f, 1.0f), tranform);
	vertex_out.color = color;

	return vertex_out;
}