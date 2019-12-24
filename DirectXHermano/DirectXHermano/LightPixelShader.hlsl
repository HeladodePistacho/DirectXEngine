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

texture2D difuse_color_texture : register(t0);
texture2D normals_texture : register(t1);
texture2D position_texture : register(t2);

SamplerState samplerstate;

cbuffer camera_buffer : register(b0)
{
	float3 camera_position;
}

float4 DoDirectional(VSOUT vertex_out)
{
	//Variables we need
	float ambient = 0.1f;

	float3 mesh_position = position_texture.Sample(samplerstate, vertex_out.texture_coords).xyz;
	float3 vertex_normal = normals_texture.Sample(samplerstate, vertex_out.texture_coords).xyz;
	float4 albedo_color = difuse_color_texture.Sample(samplerstate, vertex_out.texture_coords);

	//Vectors
	float3 normalized_direction = normalize(vertex_out.direction);
	float3 cam_to_pos = normalize(camera_position - mesh_position);
	float3 H_vector = normalize(normalized_direction + cam_to_pos);

	//Colors
	float4 ambient_color = mul(albedo_color, ambient);
	float4 difuse_color = mul(albedo_color, max(dot(normalized_direction, vertex_normal), 0.0f));
	float4 specular_color = 0.5f * pow(max(dot(H_vector, vertex_normal), 0.0f), 2.0f);


	float4 final_color = (ambient_color + difuse_color + specular_color) * float4(vertex_out.color.rgb, 1.0f);
	final_color.a = 1.0f;

	return final_color;
}

float4 main(VSOUT vertex_out) : SV_Target
{
	if (vertex_out.type == 0.0f)
		return DoDirectional(vertex_out);
	else return float4(vertex_out.color.rgb, 1.0f);

	
}