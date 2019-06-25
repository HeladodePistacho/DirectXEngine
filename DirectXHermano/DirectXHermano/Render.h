#pragma once
#include "Fast.h"
#include <d3d11.h>

class Render
{
public:
	Render(HWND window_handle);
	Render(const Render&) = delete;
	~Render();

	Render& operator=(const Render&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

private:
	ID3D11Device*		 direct_device = nullptr;
	IDXGISwapChain*		 direct_swap = nullptr;
	ID3D11DeviceContext* direct_context = nullptr;
	ID3D11RenderTargetView* direct_render_target = nullptr;
};