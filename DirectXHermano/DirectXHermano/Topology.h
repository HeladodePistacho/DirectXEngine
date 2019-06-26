#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY _type) : type(_type) { }

	void Bind(Render& ren) override { GetContext(ren)->IASetPrimitiveTopology(type); }

private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};