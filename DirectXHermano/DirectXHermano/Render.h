#pragma once
#include "Fast.h"
#include <d3d11.h>
#include <wrl.h>

class Render
{
public:
	Render(HWND window_handle);
	Render(const Render&) = delete;
	~Render() = default;

	Render& operator=(const Render&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

	void DrawTestTriangle();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> direct_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> direct_swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> direct_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> direct_render_target;
};