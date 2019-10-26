#include "App.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include <math.h>

//imgui
#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

DirectXApp::DirectXApp() : imgui_manager(), window(1920, 1080, "DirectX Engine")
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

	Entity* ent = &scene->AddEntity();
	ent->GetMeshRenderer().SetMesh(&resource_manager->GetCube());

	scene->AddEntity();
	scene->AddEntity();

	//Messages queue
	int get_result = 0;

	while (true)
	{	
		if (Window::ProcessMessages() != MESSAGE_OK)
			return 1;
	
		BeginFrame();
		Update(1.0f);
		Draw(1.0f);
		EndFrame();
	}

	return 3;
}

void DirectXApp::BeginFrame()
{
	window.GetRender().ClearBuffer(0.5f, 0.5f, 0.5f);

	//Begin Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Check for Dropped Files
	while (window.dropper.AnyDroppedFiles())
	{
		resource_manager->ImportResource(window.dropper.GetFile(), window.GetRender());
		window.dropper.RemoveFile();
	}
}

void DirectXApp::Update(float dt)
{	
	CameraControls();
	scene->Update();

	//Set Camera
	if (scene_camera->needs_update)
	{
		window.GetRender().SetCamera((*scene_camera));
	}
}

void DirectXApp::EndFrame()
{
	//ImGui
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	window.GetRender().EndFrame();
}

void DirectXApp::Draw(float dt)
{
	resource_manager->GetShader().Bind(window.GetRender());
	window.GetRender().DrawTestTriangle(
		timer.Peek(),
		((window.mouse.GetPosX() / 784.0f) * 2.0f) - 1.0f,
		((window.mouse.GetPosY() / 561.0f) * -2.0f) + 1.0f
	);
	
	scene->Draw(window.GetRender());
	scene->DrawUI(*resource_manager);
}

void DirectXApp::CameraControls()
{
	float yaw = scene_camera->yaw;
	float pitch = scene_camera->pitch;
	float roll = scene_camera->roll;

	//Panning
	if (window.keyboard.KeyIsPressed('W'))
		scene_camera->Move(-cos(yaw) * sin(pitch),
							sin(yaw),
						   -cos(yaw) * cos(pitch));

	if (window.keyboard.KeyIsPressed('S'))
		scene_camera->Move( cos(yaw) * sin(pitch),
							-sin(yaw) ,
							cos(yaw) * cos(pitch));

	if (window.keyboard.KeyIsPressed('A'))
		scene_camera->Move(	cos(yaw) * cos(pitch),
							0.0f,
							cos(yaw) * -sin(pitch));

	if (window.keyboard.KeyIsPressed('D'))
		scene_camera->Move( -cos(yaw) * cos(pitch),
							0.0f,
							cos(yaw) * sin(pitch));

	if (window.keyboard.KeyIsPressed('Q'))
		scene_camera->Move(0.0f, -0.5f, 0.0f);

	if (window.keyboard.KeyIsPressed('E'))
		scene_camera->Move(0.0f, 0.5f, 0.0f);

	if (window.keyboard.KeyIsPressed('R'))
		scene_camera->ResetRotation();

	
	if (window.mouse.LeftIsPressed() && window.keyboard.KeyIsPressed(VK_SPACE))
	{
     	int mouse_x = window.mouse.GetPosX();
		int mouse_y = window.mouse.GetPosY();
		
		int mouse_delta_x = last_mouse_pos_x - mouse_x;
		int mouse_delta_y = last_mouse_pos_y - mouse_y;

		scene_camera->Rotate(-mouse_delta_y, -mouse_delta_x);
	}

	scene_camera->DrawUI();

	//mouse stuff
	last_mouse_pos_x = window.mouse.GetPosX();
	last_mouse_pos_y = window.mouse.GetPosY();
}
