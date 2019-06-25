#include "App.h"

DirectXApp::DirectXApp() : window(800, 600, "DirectX Engine")
{
	
}

int DirectXApp::Start()
{
	//Messages queue
	int get_result = 0;
	while (true)
	{	
		if (Window::ProcessMessages() != MESSAGE_OK)
			return -1;

		if (window.keyboard.KeyIsPressed(VK_SPACE))
			return 2;
		
		Update(1.0f);
	}

	return 3;
}

void DirectXApp::Update(float dt)
{
	//float time = (sin(timer.Peek()) / 2.0) + 0.5;
	window.GetRender().ClearBuffer(0.5f, 0.5f, 0.5f);
	window.GetRender().EndFrame();
}
