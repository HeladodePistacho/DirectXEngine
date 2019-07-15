#include "App.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Mesh.h"
#include "ShaderProgram.h"

DirectXApp::DirectXApp() : window(800, 600, "DirectX Engine")
{
	scene_camera = new Camera(window.GetWidth(), window.GetHeight());
	scene = new Scene();
	resource_manager = new ResourceManager();
}

int DirectXApp::Start()
{
	//Load Resources Before Loop
	resource_manager->Start(window.GetRender());
	scene_camera->needs_update = true;

	//Messages queue
	int get_result = 0;

	while (true)
	{	
		if (Window::ProcessMessages() != MESSAGE_OK)
			return -1;

		if (window.keyboard.KeyIsPressed(VK_SPACE))
			return 2;
		
		Update(1.0f);
		Draw(1.0f);
	}

	return 3;
}

void DirectXApp::Update(float dt)
{
	window.GetRender().ClearBuffer(0.5f, 0.5f, 0.5f);
	
	scene->Update();

	//Set Camera
	if (scene_camera->needs_update)
	{
		window.GetRender().SetCamera((*scene_camera));
		scene_camera->needs_update = false; 
	}
}

void DirectXApp::Draw(float dt)
{
	resource_manager->GetShader().Bind(window.GetRender());
	window.GetRender().DrawTestTriangle(
		timer.Peek(),
		((window.mouse.GetPosX() / 784.0f) * 2.0f) - 1.0f,
		((window.mouse.GetPosY() / 561.0f) * -2.0f) + 1.0f
	);
	
	//scene->Draw(window.GetRender());

	
	resource_manager->GetCube().Draw(window.GetRender());

	window.GetRender().EndFrame();
}
