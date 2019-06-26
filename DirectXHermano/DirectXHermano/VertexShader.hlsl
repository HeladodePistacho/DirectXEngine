
struct VSOUT
{
	//float3 color : Color;
	float4 position : SV_Position;
};

//cbuffer constant_buffer
//{
//	matrix tranform;
//};

VSOUT main(float3 position : Position)
{
	VSOUT vertex_out;
	vertex_out.position = float4(position.x, position.y, 0.0f, 1.0f);

	return vertex_out;
}