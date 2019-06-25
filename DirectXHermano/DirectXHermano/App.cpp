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
	window.GetRender().ClearBuffer(0.5f, 0.5f, 0.5f);
	window.GetRender().DrawTestTriangle(
		timer.Peek(), 
		((window.mouse.GetPosX() / 784.0f) * 2.0f) - 1.0f,
		((window.mouse.GetPosY() / 561.0f) * -2.0f) + 1.0f
	);
	window.GetRender().EndFrame();
}
