struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

float4 main(VSOUT vertex_out) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}