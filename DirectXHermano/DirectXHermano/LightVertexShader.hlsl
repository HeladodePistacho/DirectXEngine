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

	float3 light_position : LIGHT_POSITION;
	float1 intensity : LIGHT_INTENSITY;

	float3 direction : LIGHT_DIRECTION;
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
};

VSOUT DoDirectional(VSIN vertex_in)
{
	VSOUT ret;

	ret.type = type;
	ret.color = color;
	ret.direction = direction;
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


	return ret;
}

VSOUT DoPoint()
{
	VSOUT ret;
	
	ret.position = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return ret;
}

VSOUT main(VSIN vertex_in)
{
	if (type == 0.0f)
		return DoDirectional(vertex_in);
	else return DoPoint();

}