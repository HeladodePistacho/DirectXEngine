#pragma once
#include "Bindable.h"

class BlendState : public Bindable
{
public:
	BlendState(Render& ren);
	~BlendState();

	void Bind(Render& ren);

private:
	ID3D11BlendState* blend_state = nullptr;
	float blend_factor[4] = { 1, 1, 1, 1 };
};