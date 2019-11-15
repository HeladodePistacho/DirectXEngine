struct VSIN
{
	float3 position : Position;
	float3 normal : Normal;
	float2 tex_coords : TextureCoords;
};

struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

VSOUT main(VSIN vertex_in)
{
	VSOUT vertex_out;
	vertex_out.position = float4(vertex_in.position, 1.0);
	vertex_out.texture_coords = vertex_in.tex_coords;

	return vertex_out;
}