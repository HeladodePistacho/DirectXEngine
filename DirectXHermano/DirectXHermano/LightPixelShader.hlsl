struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

texture2D difuse_color_texture : register(t0);
texture2D normals_texture : register(t1);
texture2D position_texture : register(t2);

SamplerState samplerstate;

cbuffer light_buffer : register(b0)
{
	float3 color;
	float1 type;

	float3 position;
	float1 intensity;

	float3 direction;
};

cbuffer camera_buffer : register(b1)
{
	float3 camera_position;
}

float4 main(VSOUT vertex_out) : SV_Target
{
	//Variables we need
	float ambient = 0.1f;

	float3 mesh_position = position_texture.Sample(samplerstate, vertex_out.texture_coords).xyz;
	float3 vertex_normal = normals_texture.Sample(samplerstate, vertex_out.texture_coords).xyz;
	float4 albedo_color = difuse_color_texture.Sample(samplerstate, vertex_out.texture_coords);

	//Vectors
	float3 pos_to_light = normalize(mesh_position - direction);
	float3 cam_to_pos = normalize(mesh_position - camera_position);

	//Add ambient
	float4 final_color = mul(albedo_color, ambient);

	final_color += mul(albedo_color, dot(pos_to_light, vertex_normal));// + pow(dot(vertex_normal, normalize(pos_to_light + cam_to_pos)), 2.0f);
	

	final_color = final_color * float4(color.rgb, 1.0f);

	return final_color;
}