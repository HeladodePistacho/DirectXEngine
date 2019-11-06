#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Render& ren);
	~Sampler();

	void Bind(Render& ren) override;

private:
	ID3D11SamplerState* sampler_state = nullptr;
};