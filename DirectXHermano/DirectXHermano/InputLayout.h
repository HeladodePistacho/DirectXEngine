#pragma once
#include "Bindable.h"
#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(Render& ren, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertex_shader_bytecode);
	~InputLayout();

	void Bind(Render& ren) override;

private:
	ID3D11InputLayout* input_layout = nullptr;
};