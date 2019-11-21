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
	void DrawScreen();
};