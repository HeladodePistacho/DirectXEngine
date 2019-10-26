
struct VSOUT
{
	float3 color : Color;
	float4 position : SV_Position;
};

cbuffer trans_buffer
{
	matrix model_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

VSOUT main(float3 position : Position, float3 normal : Normal)
{
	matrix model_view = mul(model_matrix, view_matrix);
	matrix model_view_projection = mul(model_view, projection_matrix);

	VSOUT vertex_out;
	vertex_out.position = mul(float4(position, 1.0f), model_view_projection);
	vertex_out.color = float3(normal);

	return vertex_out;
}