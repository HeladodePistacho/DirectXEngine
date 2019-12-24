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

DirectXApp::~DirectXApp()
{
	delete scene_camera;
	delete scene;
	delete resource_manager;
}

int DirectXApp::Start()
{
	//Load Resources Before Loop
	resource_manager->Start(window.GetRender());
	scene_camera->needs_update = true;

	scene->AddEntity();
	scene->AddEntity().CreateLightComponent();

	//Messages queue
	int get_result = 0;

	while (true)
	{	
		if (Window::ProcessMessages() != MESSAGE_OK)
			return 1;
	
		BeginFrame();
		Update(framerate_ms);
		Draw(1.0f);
		EndFrame();
	}

	return 3;
}

void DirectXApp::BeginFrame()
{
	timer.Mark();

	//imgui Begin Frame
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
	render_timer.Mark();
	CameraControls(dt);
	scene->Update();

	//Set Camera
	if (scene_camera->needs_update)
	{
		window.GetRender().SetCamera((*scene_camera));
		scene_camera->UpdateCameraBuffer(window.GetRender());
		scene_camera->UpdateCameraMatrixBuffer(window.GetRender());
	}
	update_time = render_timer.Peek();

}

void DirectXApp::EndFrame()
{
	//ImGui
	render_timer.Mark();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	window.GetRender().EndFrame();
	end_frame_time = render_timer.Peek();

	framerate_ms = timer.Peek();
}

void DirectXApp::Draw(float dt)
{
	//Create Deferred textures
	render_timer.Mark();
	DoDeferred();
	deferred_time = render_timer.Peek();

	//Draw Lights
	DoLights();
	
	//Draw Grid
	DoGrid();

	//Draw screen plane
	render_timer.Mark();
	DrawScreen();
	screen_draw_time = render_timer.Peek();

	//Draw UI
	render_timer.Mark();
	DrawApplicaionUI();
	DrawSceneUI();
	DrawMaterialEditorUI();
	ui_draw_time = render_timer.Peek();
}

void DirectXApp::CameraControls(float dt)
{
	float yaw = scene_camera->yaw;
	float pitch = scene_camera->pitch;
	float roll = scene_camera->roll;

	//Panning
	if (window.keyboard.KeyIsPressed('W'))
		scene_camera->Move(cos(yaw) * sin(pitch),
							-sin(yaw),
						   cos(yaw) * cos(pitch), dt);

	if (window.keyboard.KeyIsPressed('S'))
		scene_camera->Move( -cos(yaw) * sin(pitch),
							sin(yaw) ,
							-cos(yaw) * cos(pitch), dt);

	if (window.keyboard.KeyIsPressed('A'))
		scene_camera->Move(	-cos(yaw) * cos(pitch),
							0.0f,
							-cos(yaw) * -sin(pitch), dt);

	if (window.keyboard.KeyIsPressed('D'))
		scene_camera->Move( cos(yaw) * cos(pitch),
							0.0f,
							-cos(yaw) * sin(pitch), dt);

	if (window.keyboard.KeyIsPressed('Q'))
		scene_camera->Move(0.0f, 0.5f, 0.0f, dt);

	if (window.keyboard.KeyIsPressed('E'))
		scene_camera->Move(0.0f, -0.5f, 0.0f, dt);

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

	if(show_camera_window)
		scene_camera->DrawUI();

	//mouse stuff
	last_mouse_pos_x = window.mouse.GetPosX();
	last_mouse_pos_y = window.mouse.GetPosY();
}

//------------------------------ RENDER STUFF ---------------------------------------

void DirectXApp::DoDeferred()
{
	//Set Render Targets
	window.GetRender().SetDeferredRenderBuffers();

	//Clear Buffers
	window.GetRender().ClearDeferredBuffers();

	//Bind mesh Shader
	resource_manager->GetShader().Bind(window.GetRender());

	//Draw Geometry
	mesh_timer.Mark();
	scene->Draw(window.GetRender());
	mesh_draw_time = mesh_timer.Peek();

	//Return to default
	window.GetRender().SetDefaultRenderTarget();
}

void DirectXApp::DoLights()
{
	//Set Render Shaded Target -> 3 for final image
	window.GetRender().SetDeferredRenderBuffer(3);

	//Clear the Render Target
	window.GetRender().ClearDeferredBuffer(3);

	//Bind Light Shader
	resource_manager->light_shader->Bind(window.GetRender());

	//Enable Blending
	window.GetRender().BindLightBlending();

	//Bind Deferred Textures
	window.GetRender().BindDeferredTexture(0, 0); //Diffuse texture, slot 0
	window.GetRender().BindDeferredTexture(1, 1); //Normal texture, slot 1
	window.GetRender().BindDeferredTexture(2, 2); //Position texture, slot 2

	//Bind Buffers
	scene_camera->camera_buffer->BindSlot(window.GetRender(), 1u);

	//Bind Sampler
	window.GetRender().BindDeferredSampler();

	//Only Directional lights for the moment
	for(int i = 0; i < scene->GetNumLights(); i++)
	{
		scene->DrawLightAt(i, window.GetRender());

		switch (scene->GetLightType(i))
		{
		case 0: //Directional Light
			resource_manager->screen_mesh->DrawAll(window.GetRender());
			break;
			
		case 1: //Point Light
			resource_manager->sphere_mesh->DrawAll(window.GetRender());
			break;
		}
			
	}
	
	//Disable Blending
	window.GetRender().BindDefautBlending();

	//Return to Default
	window.GetRender().SetDefaultRenderTarget();
}

void DirectXApp::DoGrid()
{
	//Set Render Shaded Target -> 3 for final image
	window.GetRender().SetDeferredRenderBuffer(3);

	//Bind Grid Shder
	resource_manager->grid_shader->Bind(window.GetRender());

	//Enable Blending
	window.GetRender().BindLightBlending();

	//Bind Deferred Textures
	window.GetRender().BindDepthTexture();

	//Bind Sampler
	window.GetRender().BindDeferredSampler();

	//Bind Buffers
	scene_camera->camera_buffer->BindSlot(window.GetRender());
	scene_camera->camera_matrixs_buffer->BindSlot(window.GetRender(), 1u);

	//Draw quad mesh
	resource_manager->screen_mesh->DrawAll(window.GetRender());


	//Disable Blending
	window.GetRender().BindDefautBlending();

	//Return to Default
	window.GetRender().SetDefaultRenderTarget();

}

void DirectXApp::DrawScreen()
{
	//Clear main buffer
	window.GetRender().ClearBuffer(0.5f, 0.5f, 0.5f);

	//Bind screen shader
	resource_manager->screen_shader->Bind(window.GetRender());

	//Bind Color texture
	ID3D11ShaderResourceView* tmp_texture = nullptr;
	switch (texture_type)
	{
	case 0:
		//Albedo
		tmp_texture = window.GetRender().deferred_buffers->GetShaderResourceView(0);
		break;

	case 1:
		//Normals
		tmp_texture = window.GetRender().deferred_buffers->GetShaderResourceView(1);
		break;

	case 2:
		//Position
		tmp_texture = window.GetRender().deferred_buffers->GetShaderResourceView(2);
		break;

	case 3:
		//Depth
		tmp_texture = window.GetRender().deferred_buffers->GetShaderResourceView(3);
		break;

	case 4:
		//Depth
		tmp_texture = window.GetRender().deferred_buffers->GetDepthView();
		break;
	} 
	window.GetRender().direct_context->PSSetShaderResources(0u, 1u, &tmp_texture);

	ID3D11SamplerState* tmp_sampler = window.GetRender().deferred_buffers->GetSamplerState();
	window.GetRender().direct_context->PSSetSamplers(0u, 1u, &tmp_sampler);

	resource_manager->screen_mesh->DrawAll(window.GetRender());
}

//------------------------------ UI -------------------------------------------------

void DirectXApp::DrawApplicaionUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Application"))
		{
			if (ImGui::MenuItem("Camera Window"))
				show_camera_window = !show_camera_window;

			if (ImGui::MenuItem("Material Editor Window"))
				show_material_editor = !show_material_editor;

			if (ImGui::MenuItem("Scene Window"))
				show_scene_window = !show_scene_window;

			ImGui::Separator();

			ImGui::Text("Perfomance: ");
			ImGui::Text("Frame MS: %.4f", framerate_ms);
			ImGui::Text("Framrate: %i", (int)(1.0f / framerate_ms));

			ImGui::Text("Time Update: %.4f", update_time);
			ImGui::Text("Time Deferred 1st pass: %.4f", deferred_time);
			ImGui::Text("Time Mesh drawing: %.4f", mesh_draw_time);
			ImGui::Text("Time Screen Drawing: %.4f", screen_draw_time);
			ImGui::Text("Time UI Drawing: %.4f", ui_draw_time);
			ImGui::Text("Time Ending Frame: %.4f", end_frame_time);

			ImGui::Separator();
			ImGui::Text("Total Time: %.4f", update_time + deferred_time + ui_draw_time + screen_draw_time + end_frame_time);


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Render"))
		{
			if (ImGui::BeginMenu("Visible Texture"))
			{
				if (ImGui::Selectable("Albedo", false, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, ImVec2(100, 0)))
					texture_type = 0;

				if (ImGui::Selectable("Normals", false, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, ImVec2(100, 0)))
					texture_type = 1;

				if (ImGui::Selectable("Position", false, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, ImVec2(100, 0)))
					texture_type = 2;

				if (ImGui::Selectable("Final Result", false, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, ImVec2(100, 0)))
					texture_type = 3;

				if (ImGui::Selectable("Depth", false, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, ImVec2(100, 0)))
					texture_type = 4;
				
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		
		ImGui::Text("Framrate: %i", (int)(1.0f / framerate_ms));
		

		ImGui::EndMainMenuBar();
	}
}

void DirectXApp::DrawSceneUI()
{
	if(show_scene_window)
		scene->DrawUI(*resource_manager);
}

void DirectXApp::DrawMaterialEditorUI()
{
	if(show_material_editor)
		resource_manager->DrawMaterialEditorUI(window.GetRender());
}
