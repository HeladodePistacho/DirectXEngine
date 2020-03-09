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

	float3 color : LIGHT_COLOR;
	float1 type : LIGHT_TYPE;

	float3 light_center : LIGHT_CENTER;
	float1 intensity : LIGHT_INTENSITY;

	float3 direction : LIGHT_DIRECTION;
	float radius : LIGHT_SCALE;
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
	
	VSOUT ret;

	ret.type = type;
	ret.color = color;
	ret.direction = direction;
	ret.intensity = intensity;
	ret.texture_coords = vertex_in.tex_coords;

	//Change texture coords to ndc coords
	/*
	[-1, 1]------------------[ 1, 1]
	|						|
	|						|
	|						|
	|						|
	[-1, -1]-----------------[ 1,-1]
	*/

	ret.position = float4(ret.texture_coords.x * 2 - 1, (1 - ret.texture_coords.y) * 2 - 1, 0, 1);

	ret.light_center = position;
	ret.radius = radius;

	return ret;
}