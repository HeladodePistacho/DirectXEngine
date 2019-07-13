
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
};

cbuffer trans_buffer
{
	matrix view_matrix;
	matrix projection_matrix;
};

VSOUT main(float3 position : Position)
{
	matrix mvp = mul(view_matrix, projection_matrix);

	VSOUT vertex_out;
	vertex_out.position = mul(float4(position.x, position.y, 3.0f, 1.0f), mvp);
	vertex_out.color = float3(projection_matrix[0].xyz);

	return vertex_out;
}