#pragma once

#include "Window.h"
#include "Timer.h"

class Scene;

class DirectXApp
{
public:
	DirectXApp();

	int Start();
private:
	Timer timer;
	Window window;
	Scene* scene;

	void Update(float dt);
	void Draw(float dt);
};