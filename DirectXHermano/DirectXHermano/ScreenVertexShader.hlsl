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
	
	vertex_out.texture_coords = vertex_in.tex_coords;
	//Change texture coords to ndc coords
	/*
		[-1, 1]------------------[ 1, 1]
			|						|
			|						|
			|						|
			|						|
		[-1, -1]-----------------[ 1,-1]
	*/
	vertex_out.position = float4(vertex_out.texture_coords.x * 2 - 1, (1 - vertex_out.texture_coords.y) * 2 - 1, 0, 1);

	return vertex_out;
}