#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Render & ren, std::vector<Vertex> vertices)
{
	D3D11_BUFFER_DESC buffer_descriptor = {};
	buffer_descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_descriptor.Usage = D3D11_USAGE_DEFAULT;
	buffer_descriptor.CPUAccessFlags = 0u;
	buffer_descriptor.MiscFlags = 0u;
	buffer_descriptor.ByteWidth = sizeof(Vertex) * vertices.size();
	buffer_descriptor.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertices_data = {};
	vertices_data.pSysMem = vertices.data();

	//Create the buffer
	HRESULT hr;
	if (FAILED(hr = GetDevice(ren)->CreateBuffer(&buffer_descriptor, &vertices_data, &buffer_data)))
	{
		custom_exception error("Render Error", "Triangle Buffer Creation Failed");
		throw error;
	}
}

VertexBuffer::~VertexBuffer()
{
	if (buffer_data != nullptr)
		buffer_data->Release();
}

void VertexBuffer::Bind(Render& ren)
{
	//Bind Buffer
	if (buffer_data != nullptr)
	{
		UINT stride_size = (UINT)sizeof(Vertex);
		const UINT offset = 0u;
		GetContext(ren)->IASetVertexBuffers(0u, 1u, &buffer_data, &stride_size, &offset);
	}
	
}
