struct VSIN
{
	float3 position : Position;
	float3 normal : Normal;
	float2 tex_coords : TextureCoords;
};

struct VSOUT
{
	float3 normal : Normal;
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

cbuffer trans_buffer
{
	matrix model_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

VSOUT main(VSIN vertex_shader_in)
{
	matrix model_view = mul(model_matrix, view_matrix);
	matrix model_view_projection = mul(model_view, projection_matrix);

	VSOUT vertex_out;
	vertex_out.position = mul(float4(vertex_shader_in.position, 1.0f), model_view_projection);
	vertex_out.normal = float3(vertex_shader_in.normal);
	vertex_out.texture_coords = vertex_shader_in.tex_coords;

	return vertex_out;
}