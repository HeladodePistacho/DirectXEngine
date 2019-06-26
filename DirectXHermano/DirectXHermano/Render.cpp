#include "Render.h"
#include "ErrorHandling.h"
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Dx = DirectX;

Render::Render(HWND window_handle)
{
	custom_exception error("Render Error", "Unknown Error");

	//Holds information configuration for D3D
	DXGI_SWAP_CHAIN_DESC descriptor;
	descriptor.BufferDesc.Width = 0;
	descriptor.BufferDesc.Height = 0;
	descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descriptor.BufferDesc.RefreshRate.Numerator = 0;
	descriptor.BufferDesc.RefreshRate.Denominator = 0;
	descriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	descriptor.SampleDesc.Count = 1;
	descriptor.SampleDesc.Quality = 0;
	descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descriptor.BufferCount = 1;
	descriptor.OutputWindow = window_handle;
	descriptor.Windowed = TRUE;
	descriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	descriptor.Flags = 0;

	//Create the Device, the swap chain and the rendring context
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &descriptor, &direct_swap, &direct_device, nullptr, &direct_context)))
	{ 
		error.Fill("Render Error", "Error while Creating the Device and Swap Chain");
		throw error;
	}

	//Get acces to swap chain texture
	Microsoft::WRL::ComPtr<ID3D11Resource> swap_back_buffer;
	if (FAILED(direct_swap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&swap_back_buffer)))
	{
		error.Fill("Render Error", "Error while Getting the Buffer from Swap Chain");
		throw error;
	}
	
	if (FAILED(direct_device->CreateRenderTargetView(swap_back_buffer.Get(), nullptr, &direct_render_target)))
	{
		error.Fill("Render Error", "Error while Creating Render Target View");
		throw error;
	}

	direct_context->OMSetRenderTargets(1u, direct_render_target.GetAddressOf(), nullptr);

	D3D11_VIEWPORT view_port;
	view_port.Width = 784;
	view_port.Height = 561;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	direct_context->RSSetViewports(1u, &view_port);
	
}

void Render::EndFrame()
{
	HRESULT hr;

	//Swaps buffers -> 1u = 60fps, 2u = 30fps
	if (FAILED(hr = direct_swap->Present(1u, 0u)))
	{
		custom_exception error("","");
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
			error.Fill("Render Error", "Device Removed, Possible Drivers Issue");
		else error.Fill("Render Error", "Swapping Buffers Error");

		throw error;
	}
}

void Render::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0 };
	direct_context->ClearRenderTargetView(direct_render_target.Get(), color);
}

void Render::DrawTestTriangle(float angle, float x, float y)
{
	//Load Vertex buffer data
	ID3D11Buffer* vertex_buffer = nullptr;

	struct Vertexo
	{
		//Pos
		float x, y;

		//Color
		unsigned char r, g, b, a;
	};

	const Vertexo vertex_data[] =
	{   //Position    //Color
		{ -1.0f, 0.0f, 255, 0, 0, 255 },
		{ -1.0f, 1.0f, 0, 0, 255, 255 },
		{ 0.0f, 0.0f,  0, 255, 0, 255 },
		{ 0.0f, 1.0f,  255, 255, 255, 255 }
	};

	D3D11_BUFFER_DESC buffer_descriptor = {};
	buffer_descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_descriptor.Usage = D3D11_USAGE_DEFAULT;
	buffer_descriptor.CPUAccessFlags = 0u;
	buffer_descriptor.MiscFlags = 0u;
	buffer_descriptor.ByteWidth = sizeof(vertex_data);
	buffer_descriptor.StructureByteStride = sizeof(Vertexo);

	D3D11_SUBRESOURCE_DATA vertices = {};
	vertices.pSysMem = vertex_data;

	//Create the buffer
	if (FAILED(direct_device->CreateBuffer(&buffer_descriptor, &vertices, &vertex_buffer)))
	{
		custom_exception error("Render Error", "Triangle Buffer Creation Failed");
		throw error;
	}

	//Bind Verex Buffer
	const UINT offset = 0u;
	direct_context->IASetVertexBuffers(0u, 1u, &vertex_buffer, &buffer_descriptor.StructureByteStride, &offset);

	//Create Indices Buffer
	const unsigned short indices_data[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	ID3D11Buffer* indices_buffer = nullptr;
	D3D11_BUFFER_DESC indices_descriptor = {};
	indices_descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indices_descriptor.Usage = D3D11_USAGE_DEFAULT;
	indices_descriptor.CPUAccessFlags = 0u;
	indices_descriptor.MiscFlags = 0u;
	indices_descriptor.ByteWidth = sizeof(indices_data);
	indices_descriptor.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indices = {};
	indices.pSysMem = indices_data;

	//Create the buffer
	if (FAILED(direct_device->CreateBuffer(&indices_descriptor, &indices, &indices_buffer)))
	{
		custom_exception error("Render Error", "Triangle Indices Buffer Creation Failed");
		throw error;
	}

	//Bind Index Buffer
	direct_context->IASetIndexBuffer(indices_buffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0u);

	//Create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		Dx::XMMATRIX transform;
	};

	const ConstantBuffer matrix =
	{
		{
			Dx::XMMatrixTranspose(
				Dx::XMMatrixRotationZ(angle) *
				Dx::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
				Dx::XMMatrixTranslation(x, y, 0.0f)
			)
		}
	};
	
	ID3D11Buffer* transform_buffer = nullptr;
	D3D11_BUFFER_DESC transform_descriptor = {};
	transform_descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transform_descriptor.Usage = D3D11_USAGE_DYNAMIC;
	transform_descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transform_descriptor.MiscFlags = 0u;
	transform_descriptor.ByteWidth = sizeof(matrix);
	transform_descriptor.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA transform_data = {};
	transform_data.pSysMem = &matrix;

	if (FAILED(direct_device->CreateBuffer(&transform_descriptor, &transform_data, &transform_buffer)))
	{
		custom_exception error("Render Error", "Triangle Indices Buffer Creation Failed");
		throw error;
	}

	direct_context->VSSetConstantBuffers(0u, 1u, &transform_buffer);

	//Create Vertex Shader
	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* blob = nullptr;
	D3DReadFileToBlob(L"VertexShader.cso", &blob);
	direct_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader);

	//Bind Vertex Shader
	direct_context->VSSetShader(vertex_shader, nullptr, 0u);
	
	//Create Pixel Shader
	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* blob_2 = nullptr;
	D3DReadFileToBlob(L"PixelShader.cso", &blob_2);
	direct_device->CreatePixelShader(blob_2->GetBufferPointer(), blob_2->GetBufferSize(), nullptr, &pixel_shader);

	//Bind Pixel Shader
	direct_context->PSSetShader(pixel_shader, nullptr, 0u);
	
	//Tells the vertex shader how is structured the vertex input data
	ID3D11InputLayout* input_layout = nullptr;
	const D3D11_INPUT_ELEMENT_DESC input_descriptor[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(float)*2, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	direct_device->CreateInputLayout(input_descriptor, (UINT)std::size(input_descriptor), blob->GetBufferPointer(), blob->GetBufferSize(), &input_layout);

	//Bind Vertex Layout
	direct_context->IASetInputLayout(input_layout);

	//Bind Render Target
	direct_context->OMSetRenderTargets(1u, direct_render_target.GetAddressOf(), nullptr);

	direct_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT view_port;
	view_port.Width = 784;
	view_port.Height = 561;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	view_port.TopLeftX = 0; 
	view_port.TopLeftY = 0;
	direct_context->RSSetViewports(1u, &view_port);

	direct_context->DrawIndexed((UINT)std::size(indices_data), 0u, 0u );
}
