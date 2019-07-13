#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Render & ren, const std::vector<unsigned short>& indices) : num_indices((UINT)indices.size())
{
	//Create Index Buffer
	D3D11_BUFFER_DESC indices_descriptor = {};
	indices_descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indices_descriptor.Usage = D3D11_USAGE_DEFAULT;
	indices_descriptor.CPUAccessFlags = 0u;
	indices_descriptor.MiscFlags = 0u;
	indices_descriptor.ByteWidth = (UINT)(num_indices * sizeof(unsigned short));
	indices_descriptor.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indices_data = {};
	indices_data.pSysMem = indices.data();

	//Create the buffer
	if (FAILED(GetDevice(ren)->CreateBuffer(&indices_descriptor, &indices_data, &buffer_data)))
	{
		//custom_exception error("Render Error", "Triangle Indices Buffer Creation Failed");
		//throw error;
	}
}

IndexBuffer::~IndexBuffer()
{
	if (buffer_data != nullptr)
		buffer_data->Release();
}

void IndexBuffer::Bind(Render& ren)
{
	//Bind Index Buffer
	GetContext(ren)->IASetIndexBuffer(buffer_data, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0u);
}
