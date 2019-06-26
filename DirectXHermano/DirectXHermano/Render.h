#pragma once
#include "Fast.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class Render
{
	friend class Bindable;
public:
	Render(HWND window_handle);
	Render(const Render&) = delete;
	~Render() = default;

	Render& operator=(const Render&) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b);

	void DrawTestTriangle(float angle, float x, float y);
	void DrawIndices(UINT num_indices) { direct_context->DrawIndexed(num_indices, 0u, 0u); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> direct_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> direct_swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> direct_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> direct_render_target;
};