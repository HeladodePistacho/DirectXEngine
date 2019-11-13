#pragma once
#include "ErrorHandling.h"
#include "Render.h"

class Bindable
{
public:
	virtual void Bind(Render& ren) = 0;
	virtual void BindSlot(Render& ren, unsigned int slot = 0u) {}

	virtual ~Bindable() {};

protected:
	static ID3D11DeviceContext* GetContext(Render& ren);
	static ID3D11Device* GetDevice(Render& ren);
};