#include "BlendState.h"

BlendState::BlendState(Render & ren)
{
	D3D11_BLEND_DESC blend_descriptor = {};

	blend_descriptor.RenderTarget[0].BlendEnable = TRUE;

	blend_descriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_descriptor.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_descriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_descriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_descriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_descriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_descriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	GetDevice(ren)->CreateBlendState(&blend_descriptor, &blend_state);
}

BlendState::~BlendState()
{
	if (blend_state)
	{
		blend_state->Release();
		delete blend_state;
	}
}

void BlendState::Bind(Render & ren)
{
	GetContext(ren)->OMSetBlendState(blend_state, 0, 0xffffffff);
}
