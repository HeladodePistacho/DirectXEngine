#pragma once

#include "Window.h"
#include "Timer.h"

class Scene;
class ResourceManager;
class Camera;

class DirectXApp
{
public:
	DirectXApp();

	int Start();
private:
	Timer timer;
	Window window;

	Camera* scene_camera = nullptr;
	Scene* scene = nullptr;
	ResourceManager* resource_manager = nullptr;

	void Update(float dt);
	void Draw(float dt);
};