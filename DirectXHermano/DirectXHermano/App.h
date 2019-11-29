#pragma once

#include "Window.h"
#include "Timer.h"
#include "ImGuiManager.h"

class Scene;
class ResourceManager;
class Camera;

class DirectXApp
{
public:
	DirectXApp();
	~DirectXApp();

	int Start();
private:
	ImGuiManager imgui_manager;

	Timer timer;
	Timer render_timer;
	Timer mesh_timer;

	Window window;

	Camera* scene_camera = nullptr;
	Scene* scene = nullptr;
	ResourceManager* resource_manager = nullptr;

	void BeginFrame();
	void Update(float dt);
	void EndFrame();

	void Draw(float dt);

	int last_mouse_pos_x = 0;
	int last_mouse_pos_y = 0;
	void CameraControls();

	//Render Stuff
	void DoDeferred();
	void DoLights();
	void DrawScreen();

	//0 for color, 1 for normals, 2 for depth
	int texture_type = 0;

	//ImGui
	void DrawApplicaionUI();
	void DrawSceneUI();
	void DrawMaterialEditorUI();

	bool show_camera_window = false;
	bool show_render_window = false;
	bool show_scene_window = true;
	bool show_material_editor = true;

	float framerate_ms = 0.0f;
	float deferred_time = 0.0f;
	float mesh_draw_time = 0.0f;
	float screen_draw_time = 0.0f;
	float ui_draw_time = 0.0f;
	float update_time = 0.0f;
	float end_frame_time = 0.0f;
};