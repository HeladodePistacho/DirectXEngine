#pragma once

#include "Window.h"
#include "Timer.h"

class DirectXApp
{
public:
	DirectXApp();

	int Start();
private:
	Timer timer;
	Window window;

	void Update(float dt);
};