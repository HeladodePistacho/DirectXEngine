#include "Sampler.h"

Sampler::Sampler(Render & ren)
{
	D3D11_SAMPLER_DESC sampler_descriptor = {};

	sampler_descriptor.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_descriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_descriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_descriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_descriptor.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;

	sampler_descriptor.MipLODBias = 0.0f;
	sampler_descriptor.MinLOD = 0.0f;
	sampler_descriptor.MaxLOD = D3D11_FLOAT32_MAX;

	GetDevice(ren)->CreateSamplerState(&sampler_descriptor, &sampler_state);
}

Sampler::~Sampler()
{
	if (sampler_state)
		delete sampler_state;
}

void Sampler::Bind(Render& ren)
{
	GetContext(ren)->PSSetSamplers(0u, 1u, &sampler_state);
}