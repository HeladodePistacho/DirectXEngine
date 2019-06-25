
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
};

VSOUT main(float2 position : Position, float3 color : Color)
{
	VSOUT vertex_out;
	vertex_out.position = float4(position.x, position.y, 0.0f, 1.0f);
	vertex_out.color = color;

	return vertex_out;
}