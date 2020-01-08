struct VSIN
{
	float3 position : Position;
};

struct VSOUT
{
	float4 out_position : SV_Position;
	float4 color : light_color;
};

cbuffer matrix_buffer : register(b0)
{
	matrix model_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

cbuffer light_buffer : register(b1)
{
	float3 color;
	float1 type;

	float3 position;
	float1 intensity;

	float3 direction;
	float radius;
};

VSOUT main(VSIN vertex_in)
{
	matrix model_view = mul(model_matrix, view_matrix);
	matrix model_view_projection = mul(model_view, projection_matrix);

	float3 tmp = (vertex_in.position) * 0.2f;

	VSOUT vertex_out;
	vertex_out.out_position = mul(float4(tmp.xyz, 1.0f), model_view_projection);
	vertex_out.color = float4(color.rgb, 1.0f);

	return vertex_out;
}