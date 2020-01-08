struct VSOUT
{
	float4 out_position : SV_Position;
	float4 color : light_color;
};

float4 main(VSOUT vertex_out) : SV_Target
{
	return vertex_out.color;
}