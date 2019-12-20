struct VSOUT
{
	float4 position : SV_Position;
	float2 texture_coords : Texcoord0;
};

texture2D depth_texture;
SamplerState samplerstate;

cbuffer camera_buffer: register(b0)
{
	float3 camera_position;
}

cbuffer camera_matrix_buffer : register(b1)
{
	float4x4 inv_view_matrix;
	float4x4 inv_perspective_matrix;
}

float4 main(VSOUT vertex_out) : SV_Target
{
	float3 z_plane_normal = float3(0.0f, 1.0f, 0.0f );
	float3 z_plane_point = float3(0.0f, 0.0f, 0.0f);

	float depth = depth_texture.Sample(samplerstate, vertex_out.texture_coords).r;

	float tmp_x = (vertex_out.texture_coords.x  * 2.0f) - 1.0f;
	float tmp_y = ((1 - vertex_out.texture_coords.y)  * 2.0f) - 1.0f;


	//Change texture coordinates from clip space to view space
	float4 normalized_space_coordinates = float4(tmp_x, tmp_y, depth, 1.0f);
	float4 view_space_coordinates = mul(normalized_space_coordinates, inv_perspective_matrix);

	view_space_coordinates = view_space_coordinates / view_space_coordinates.w;

	//Change from view space to world space
	float4 world_space_coordinates = mul(view_space_coordinates, inv_view_matrix);

	float3 camera_pos_dir = normalize(world_space_coordinates.xyz - camera_position);

	float dot_dir_normal = dot(camera_pos_dir , z_plane_normal);
	float dot_cam_normal = dot(z_plane_point - camera_position, z_plane_normal);

	float d = dot_cam_normal / dot_dir_normal;

	
			
	if (d >= 0.0f)
	{
		float3 intersection_world_point = (d * camera_pos_dir) + camera_position;
		
		float2 grid = fwidth(intersection_world_point.xz) / (intersection_world_point.xz % 5.0f);
		float2 inverted_grid = -fwidth(intersection_world_point.xz) / (intersection_world_point.xz % 5.0f);

		if (grid.x < 1.0f)
			grid.x = inverted_grid.x;


		if (grid.y < 1.0f)
			grid.y = inverted_grid.y;


		float value = step(0.8f, max(grid.x, grid.y));



		return float4(value, value, value, 1.0f);
	}
			
		
	
	 return world_space_coordinates;
}