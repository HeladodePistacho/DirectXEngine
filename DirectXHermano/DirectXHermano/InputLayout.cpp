#include "InputLayout.h"

InputLayout::InputLayout(Render& ren, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob * vertex_shader_bytecode)
{
	if (FAILED(GetDevice(ren)->CreateInputLayout(layout.data(), (UINT)std::size(layout), vertex_shader_bytecode->GetBufferPointer(), vertex_shader_bytecode->GetBufferSize(), &input_layout)))
	{
		//custom_exception error("Render Error", "Input Layout Creation Failed");
		//throw error;
	}
}

InputLayout::~InputLayout()
{
	if (input_layout != nullptr)
		input_layout->Release();
}

void InputLayout::Bind(Render & ren)
{
	GetContext(ren)->IASetInputLayout(input_layout);
}
