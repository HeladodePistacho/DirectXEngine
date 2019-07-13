
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
};

float4 main(VSOUT vertex_out) : SV_Target
{
	return float4(vertex_out.color, 1.0f);
}