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
	float time = timer.Peek();
	
	char buffer[256];
	sprintf_s(buffer, "%f", time);
	//
	//window.SetWindowTitle("wasa");

}
