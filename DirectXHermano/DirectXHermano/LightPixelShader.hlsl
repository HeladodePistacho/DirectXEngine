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
	float3 vertex_normal = normalize(normals_texture.Sample(samplerstate, vertex_out.texture_coords).xyz);
	float4 albedo_color = difuse_color_texture.Sample(samplerstate, vertex_out.texture_coords);

	//Vectors
	float3 normalized_direction = normalize(vertex_out.direction);
	float3 cam_to_pos = normalize(camera_position - mesh_position);
	float3 H_vector = normalize(normalized_direction + cam_to_pos);

	//Colors
	float4 ambient_color = mul(albedo_color, ambient);
	float4 difuse_color = mul(albedo_color, max(dot(normalized_direction, vertex_normal), 0.0f));
	float4 specular_color = 0.5f * pow(max(dot(H_vector, vertex_normal), 0.0f), 64.0f);


	float4 final_color = (ambient_color + difuse_color + specular_color) * vertex_out.intensity * float4(vertex_out.color.rgb, 1.0f);
	final_color.a = 1.0f;

	return final_color;
}

float4 DoPoint(VSOUT vertex_out)
{
	//Variables we need
	float ambient = 0.1f;

	float3 mesh_position = position_texture.Sample(samplerstate, vertex_out.texture_coords).xyz;
	float3 vertex_normal = normalize(normals_texture.Sample(samplerstate, vertex_out.texture_coords).xyz);
	float4 albedo_color = difuse_color_texture.Sample(samplerstate, vertex_out.texture_coords);

	float tmp_length = length(vertex_out.light_center - mesh_position);
	float distance_factor;
	
	float attenuation;
	attenuation = saturate(1.0f - ((tmp_length * tmp_length) / (vertex_out.radius * vertex_out.radius)));
	attenuation *= vertex_out.intensity;
	

	//Vectors
	float3 pos_to_light = normalize(vertex_out.light_center - mesh_position);
	float3 cam_to_pos = normalize(camera_position - mesh_position);
	float3 H_vector = normalize(cam_to_pos + pos_to_light);

	//Colors
	float4 ambient_color = mul(albedo_color, ambient);
	float4 difuse_color = mul(albedo_color, max(dot(pos_to_light, vertex_normal), 0.0f));
	float4 specular_color = 0.5f * pow(max(dot(H_vector, vertex_normal), 0.0f), 2.0f);

	float4 final_color = (ambient_color + difuse_color + specular_color)  * attenuation * float4(vertex_out.color.rgb, 1.0f);
	final_color.a = 1.0f;

	
	return final_color;
}

float4 main(VSOUT vertex_out) : SV_Target
{
	if (vertex_out.type == 0.0f)
		return DoDirectional(vertex_out);
	else return DoPoint(vertex_out);

	
}