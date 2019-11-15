#pragma once
#include "Fast.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

class Camera;

class RenderBuffer
{
public:
	RenderBuffer(unsigned int num_targets);
	~RenderBuffer();

	bool LoadRenderTargets(ID3D11Device* device, int width, int height);
	bool LoadDepth(ID3D11Device* device, int width, int height);

	void SetRenderTargets(ID3D11DeviceContext* context);
	void ClearRenderTargets(ID3D11DeviceContext* context);

	ID3D11ShaderResourceView* GetShaderResourceView(unsigned int num_view) const;
	ID3D11SamplerState* GetSamplerState() const { return sampler_state; }

private:
	unsigned int used_targets = 0;

	ID3D11Texture2D* texture_array[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11RenderTargetView* view_array[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11ShaderResourceView* resource_view_array[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

	ID3D11SamplerState* sampler_state = nullptr;

	ID3D11DepthStencilView* depth = nullptr;
	ID3D11Texture2D* depth_texture = nullptr;
	D3D11_VIEWPORT view_port;
};

class Render
{
	friend class Bindable;
	friend class DirectXApp;
public:
	Render(HWND window_handle, int width, int height);
	Render(const Render&) = delete;
	~Render();

	Render& operator=(const Render&) = delete;

	void Start();
	void EndFrame();
	void ClearBuffer(float r, float g, float b);

	void DrawTestTriangle(float angle, float x, float y);
	void DrawIndices(UINT num_indices) { direct_context->DrawIndexed(num_indices, 0u, 0u); }

	const Camera& GetCamera() const { return (*render_camera); }
	void SetCamera(Camera& cam);

	//Deferred Functions
	void LoadDeferredRenderBuffers();
	void ClearDeferredBuffers();
	void SetDeferredRenderBuffers();

	void SetDefaultRenderTarget();

	//Deferred
	RenderBuffer* deferred_buffers = nullptr;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> direct_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> direct_swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> direct_context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> direct_render_target;

	//Depth
	ID3D11DepthStencilView* direct_depth = nullptr;
	D3D11_VIEWPORT view_port;

	Camera* render_camera = nullptr;

	void LoadSwapChain(HWND window_handle, int width, int height);
	void LoadRenderTargets(int widht, int height);
	void LoadDepthBuffer(int width, int height);
	void LoadViewport(int width, int height);

	
};