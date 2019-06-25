#include "Render.h"

#pragma comment(lib, "d3d11.lib")

Render::Render(HWND window_handle)
{
	//Holds information configuration for D3D
	DXGI_SWAP_CHAIN_DESC descriptor;
	descriptor.BufferDesc.Width = 0;
	descriptor.BufferDesc.Height = 0;
	descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descriptor.BufferDesc.RefreshRate.Numerator = 0;
	descriptor.BufferDesc.RefreshRate.Denominator = 0;
	descriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	descriptor.SampleDesc.Count = 1;
	descriptor.SampleDesc.Quality = 0;
	descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descriptor.BufferCount = 1;
	descriptor.OutputWindow = window_handle;
	descriptor.Windowed = TRUE;
	descriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	descriptor.Flags = 0;

	//Create the Device, the swap chain and the rendring context
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &descriptor, &direct_swap, &direct_device, nullptr, &direct_context);

	//Get acces to swap chain texture
	ID3D11Resource* swap_back_buffer = nullptr;
	direct_swap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&swap_back_buffer);
	direct_device->CreateRenderTargetView(swap_back_buffer, nullptr, &direct_render_target);
	swap_back_buffer->Release();
}

Render::~Render()
{
	if (direct_device != nullptr)
		direct_device->Release();

	if (direct_swap != nullptr)
		direct_swap->Release();

	if (direct_context != nullptr)
		direct_context->Release();

	if (direct_render_target != nullptr)
		direct_render_target->Release();
}

void Render::EndFrame()
{
	//Swaps buffers -> 1u = 60fps, 2u = 30fps
	direct_swap->Present(1u, 0u);
}

void Render::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0 };
	direct_context->ClearRenderTargetView(direct_render_target, color);
}
