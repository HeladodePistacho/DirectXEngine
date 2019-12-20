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

float3 GetPixelWorldPosition(float2 texture_coords)
{
	//Get NDC Values
	float depth = depth_texture.Sample(samplerstate, texture_coords).r;
	float tmp_x = (texture_coords.x  * 2.0f) - 1.0f;
	float tmp_y = ((1 - texture_coords.y)  * 2.0f) - 1.0f;

	//Change texture coordinates from ndc space to view space
	float4 normalized_space_coordinates = float4(tmp_x, tmp_y, depth, 1.0f);
	float4 view_space_coordinates = mul(normalized_space_coordinates, inv_perspective_matrix);

	view_space_coordinates = view_space_coordinates / view_space_coordinates.w;

	//Change from view space to world space
	return mul(view_space_coordinates, inv_view_matrix).xyz;
}

float GridValue(float3 world_pos, float grid_cell_size)
{
	float2 grid = fwidth(world_pos.xz) / (world_pos.xz % 5.0f);
	float2 inverted_grid = -grid;

	if (grid.x < 1.0f)
		grid.x = inverted_grid.x;


	if (grid.y < 1.0f)
		grid.y = inverted_grid.y;


	return step(0.8f, max(grid.x, grid.y));
}

float4 main(VSOUT vertex_out) : SV_Target
{
	//Change from view space to world space
	float3 world_space_coordinates = GetPixelWorldPosition(vertex_out.texture_coords);

	//Line - Plane Intersection maths
	float3 z_plane_normal = float3(0.0f, 1.0f, 0.0f);
	float3 z_plane_point = float3(0.0f, 0.0f, 0.0f);
	float3 camera_to_pos_dir = normalize(world_space_coordinates - camera_position);

	float dot_dir_normal = dot(camera_to_pos_dir, z_plane_normal);
	float dot_cam_normal = dot(z_plane_point - camera_position, z_plane_normal);

	//"d" will be used to know if there is intersection in fron of the camera and to know the intersection point
	//according to wikipedia: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	float d = dot_cam_normal / dot_dir_normal;
			
	if (d >= 0.0f)
	{
		float3 intersection_world_point = (d * camera_to_pos_dir) + camera_position;
		
		//Check if the pixel is occluded
		float camera_to_intersection_distance = length(intersection_world_point - camera_position);
		float camera_to_world_distance = length(world_space_coordinates - camera_position);

		if (camera_to_world_distance < camera_to_intersection_distance)
			clip(-1);
		else
		{
			float value = GridValue(intersection_world_point, 3.0f) * 0.5f;
			float4 out_color = float4(value, value, value, 1.0f);

			return out_color;
		}
	}
			
		
	
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}