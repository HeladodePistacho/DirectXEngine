#include "Render.h"
#include "ErrorHandling.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Dx = DirectX;

//----------------------- RENDER ----------------------------------//
Render::Render(HWND window_handle, int width, int height)
{
	//Set the SwapChain
	LoadSwapChain(window_handle, width, height);

	//Set the Render targets for deferred
	LoadRenderTargets(width, height);

	//Set Depth test
	LoadDepthBuffer(width, height);

	//Set render targets
	direct_context->OMSetRenderTargets(1u, direct_render_target.GetAddressOf(), direct_depth);

	//Set viewport settings
	LoadViewport(width, height);

	//Load Imgui
	ImGui_ImplDX11_Init(direct_device.Get(), direct_context.Get());
}

Render::~Render()
{
	ImGui_ImplDX11_Shutdown();
	
}

void Render::EndFrame()
{
	//Camera reset
	render_camera->needs_update = false;

	HRESULT hr;

	//Swaps buffers -> 1u = 60fps, 2u = 30fps
	if (FAILED(hr = direct_swap->Present(0u, 0u)))
	{
		//custom_exception error("","");
		//if (hr == DXGI_ERROR_DEVICE_REMOVED)
		//	error.Fill("Render Error", "Device Removed, Possible Drivers Issue");
		//else error.Fill("Render Error", "Swapping Buffers Error");
		//
		//throw error;
	}
}

void Render::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0 };
	direct_context->ClearRenderTargetView(direct_render_target.Get(), color);
	direct_context->ClearDepthStencilView(direct_depth, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Render::DrawTestTriangle(float angle, float x, float y)
{

}

void Render::SetCamera(Camera & cam)
{
	render_camera = &cam;
	render_camera->needs_update = true;
}

void Render::SetDeferredRenderBuffers()
{
	deferred_buffers->SetAllRenderTargets(direct_context.Get());
}

void Render::SetDeferredRenderBuffer(int target)
{
	deferred_buffers->SetRenderTarget(direct_context.Get(), target);
}

void Render::ClearDeferredBuffers()
{
	deferred_buffers->ClearRenderTargets(direct_context.Get());
}

void Render::ClearDeferredBuffer(int target)
{
	deferred_buffers->ClearRenderTarget(direct_context.Get(), target);
}

void Render::SetDefaultRenderTarget()
{
	direct_context->OMSetRenderTargets(1u, direct_render_target.GetAddressOf(), direct_depth);
	direct_context->RSSetViewports(1, &view_port);
}

void Render::LoadSwapChain(HWND window_handle, int width, int height)
{
	//Holds information configuration for D3D
	DXGI_SWAP_CHAIN_DESC descriptor;
	descriptor.BufferDesc.Width = width;
	descriptor.BufferDesc.Height = height;
	descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descriptor.BufferDesc.RefreshRate.Numerator = 0;
	descriptor.BufferDesc.RefreshRate.Denominator = 0;
	descriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	descriptor.SampleDesc.Count = 1;
	descriptor.SampleDesc.Quality = 0;
	descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descriptor.BufferCount = 2;
	descriptor.OutputWindow = window_handle;
	descriptor.Windowed = TRUE;
	descriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	descriptor.Flags = 0;

	//Create the Device, the swap chain and the rendring context
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &descriptor, &direct_swap, &direct_device, nullptr, &direct_context)))
	{
		//error.Fill("Render Error", "Error while Creating the Device and Swap Chain");
		//throw error;
	}

	//Get acces to swap chain texture
	Microsoft::WRL::ComPtr<ID3D11Resource> swap_back_buffer;
	if (FAILED(direct_swap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&swap_back_buffer)))
	{
		//error.Fill("Render Error", "Error while Getting the Buffer from Swap Chain");
		//throw error;
	}


	if (FAILED(direct_device->CreateRenderTargetView(swap_back_buffer.Get(), nullptr, &direct_render_target)))
	{
		//error.Fill("Render Error", "Error while Creating Render Target View");
		//throw error;
	}
}

void Render::LoadRenderTargets(int width, int height)
{
	//0 -> difuse color
	//1 -> normals
	//2 -> position
	//3 -> Shaded scene
	deferred_buffers = new RenderBuffer(4);
	deferred_buffers->LoadRenderTargets(direct_device.Get(), width, height);
	deferred_buffers->LoadDepth(direct_device.Get(), width, height);
}

void Render::LoadDepthBuffer(int width, int height)
{
	D3D11_DEPTH_STENCIL_DESC depth_stencil_descriptor = {};
	depth_stencil_descriptor.DepthEnable = TRUE;
	depth_stencil_descriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_descriptor.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_descriptor.StencilEnable = FALSE;

	ID3D11DepthStencilState* depth_stencil_state = nullptr;
	direct_device->CreateDepthStencilState(&depth_stencil_descriptor, &depth_stencil_state);

	//Bind depth state to context
	direct_context->OMSetDepthStencilState(depth_stencil_state, 1u);

	//Create depth texture
	ID3D11Texture2D* depth_texture;
	D3D11_TEXTURE2D_DESC depth_texture_descriptor = {};
	depth_texture_descriptor.Width = width;
	depth_texture_descriptor.Height = height;
	depth_texture_descriptor.MipLevels = 1u;
	depth_texture_descriptor.ArraySize = 1u;
	depth_texture_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	depth_texture_descriptor.SampleDesc.Count = 1u;
	depth_texture_descriptor.SampleDesc.Quality = 0u;
	depth_texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
	depth_texture_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	direct_device->CreateTexture2D(&depth_texture_descriptor, nullptr, &depth_texture);

	//Create View of depth texture
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_descriptor = {};
	depth_stencil_view_descriptor.Format = DXGI_FORMAT_D32_FLOAT;
	depth_stencil_view_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_descriptor.Texture2D.MipSlice = 0u;

	direct_device->CreateDepthStencilView(depth_texture, &depth_stencil_view_descriptor, &direct_depth);
}

void Render::LoadViewport(int width, int height)
{
	view_port.Width = width;
	view_port.Height = height;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	direct_context->RSSetViewports(1u, &view_port);
}

//----------------------------- RENDER BUFFER ---------------------------------//

RenderBuffer::RenderBuffer(unsigned int num_targets)
{
	if(num_targets <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		used_targets = num_targets;
}

RenderBuffer::~RenderBuffer()
{
	for (int i = 0; i < used_targets; i++)
	{
		delete texture_array[i];
		delete view_array[i];
		delete resource_view_array[i];
	}

	delete[] texture_array;
	delete[] view_array;
	delete[] resource_view_array;

	if (depth != nullptr)
	{
		delete depth;
		delete depth_texture;
	}
}

bool RenderBuffer::LoadRenderTargets(ID3D11Device* device, int width, int height)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC texture_descriptor = {};
	D3D11_RENDER_TARGET_VIEW_DESC view_descriptor = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC resource_descriptor = {};

	//Load the Textures
	//Set Texture Descriptor
	texture_descriptor.Width = width;
	texture_descriptor.Height = height;
	texture_descriptor.MipLevels = 1;
	texture_descriptor.ArraySize = 1;
	texture_descriptor.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	texture_descriptor.SampleDesc.Count = 1;
	texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
	texture_descriptor.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_descriptor.CPUAccessFlags = 0;
	texture_descriptor.MiscFlags = 0;

	for (int i = 0; i < used_targets; i++)
	{
		result = device->CreateTexture2D(&texture_descriptor, nullptr, &texture_array[i]);
		if (FAILED(result))
		{
			//SOMETHING WRONG WITH TEXTURES
			return false;
		}
	}

	//Load the views
	view_descriptor.Format = texture_descriptor.Format;
	view_descriptor.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	view_descriptor.Texture2D.MipSlice = 0;

	for (int i = 0; i < used_targets; i++)
	{
		result = device->CreateRenderTargetView(texture_array[i], &view_descriptor, &view_array[i]);
		if (FAILED(result))
		{
			//SOMETHING WRONG WITH VIEWS
			return false;
		}
	}

	//Load the shader resource views
	resource_descriptor.Format = texture_descriptor.Format;
	resource_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_descriptor.Texture2D.MipLevels = 1;
	resource_descriptor.Texture2D.MostDetailedMip = 0;

	for (int i = 0; i < used_targets; i++)
	{
		result = device->CreateShaderResourceView(texture_array[i], &resource_descriptor, &resource_view_array[i]);
		if (FAILED(result))
		{
			//SOMETHING WRONG WITH SHADER RESOURCE VIEWS
			return false;
		}
	}

	//Load The Sampler state
	D3D11_SAMPLER_DESC sampler_descriptor = {};
	sampler_descriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	device->CreateSamplerState(&sampler_descriptor, &sampler_state);

	//Set Up the Viewport
	view_port.Width = width;
	view_port.Height = height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;

	return true;
}

bool RenderBuffer::LoadDepth(ID3D11Device * device, int width, int height)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depth_texture_descriptor = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_descriptor = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC depth_resource_descriptor = {};

	//Load the depth texture
	depth_texture_descriptor.Width = width;
	depth_texture_descriptor.Height = height;
	depth_texture_descriptor.MipLevels = 1u;
	depth_texture_descriptor.ArraySize = 1u;
	depth_texture_descriptor.Format = DXGI_FORMAT_R16_TYPELESS; //DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_D32_FLOAT;
	depth_texture_descriptor.SampleDesc.Count = 1u;
	depth_texture_descriptor.SampleDesc.Quality = 0u;
	depth_texture_descriptor.Usage = D3D11_USAGE_DEFAULT;
	depth_texture_descriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	result = device->CreateTexture2D(&depth_texture_descriptor, nullptr, &depth_texture);

	if (FAILED(result))
		return false;

	//Load the Depth view
	depth_descriptor.Format = DXGI_FORMAT_D16_UNORM;//DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_descriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_descriptor.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depth_texture, &depth_descriptor, &depth);

	if (FAILED(result))
		return false;

	//Load the depth resource view
	depth_resource_descriptor.Format = DXGI_FORMAT_R16_UNORM;//DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depth_resource_descriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depth_resource_descriptor.Texture2D.MipLevels = 1;
	depth_resource_descriptor.Texture2D.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(depth_texture, &depth_resource_descriptor, &depth_view);

	if (FAILED(result))
		return false;

	return true;
}

void RenderBuffer::SetAllRenderTargets(ID3D11DeviceContext * context)
{
	context->OMSetRenderTargets(used_targets, view_array, depth);
	context->RSSetViewports(1, &view_port);
}

void RenderBuffer::SetRenderTarget(ID3D11DeviceContext * context, int target)
{
	if (target >= used_targets)
		target = used_targets - 1;

	context->OMSetRenderTargets(1u, &view_array[target], nullptr);
	context->RSSetViewports(1, &view_port);
}

void RenderBuffer::ClearRenderTargets(ID3D11DeviceContext * context)
{
	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	for (int i = 0; i < used_targets; i++)
	{
		context->ClearRenderTargetView(view_array[i], color);
	}

	if (depth != nullptr)
	{
		context->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	
}

void RenderBuffer::ClearRenderTarget(ID3D11DeviceContext * context, int target)
{
	if (target >= used_targets)
		target = used_targets - 1;

	float color[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	context->ClearRenderTargetView(view_array[target], color);
}

ID3D11ShaderResourceView* RenderBuffer::GetShaderResourceView(unsigned int num_view) const
{
	if (num_view < used_targets)
	{
		return resource_view_array[num_view];
	}
}




