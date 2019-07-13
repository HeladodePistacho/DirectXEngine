#pragma once
#include "Bindable.h"

enum BUFFER_TYPE
{
	UNDEFINED,
	VERTEX_SHADER_BUFFER,
	PIXEL_SHADER_BUFFER
};

template<typename CLASS>
class ConstBuffer : public Bindable
{
public:
	ConstBuffer(Render& ren, const CLASS& buffer, BUFFER_TYPE _type) : type(_type)
	{
		D3D11_BUFFER_DESC descriptor = {};
		descriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		descriptor.Usage = D3D11_USAGE_DYNAMIC;
		descriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		descriptor.MiscFlags = 0u;
		descriptor.ByteWidth = sizeof(buffer);
		descriptor.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = &buffer;

		if (FAILED(GetDevice(ren)->CreateBuffer(&descriptor, &data, &constant_buffer)))
		{
			//custom_exception error("Render Error", "Triangle Indices Buffer Creation Failed");
			//throw error;
		}
	}

	void Update(Render& ren, const CLASS& buffer)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;

		GetContext(ren)->Map(constant_buffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_subresource);

		if (constant_buffer)
		{
			delete constant_buffer;
			constant_buffer = nullptr;
		}

		memcpy(mapped_subresource.pData, constant_buffer, sizeof(buffer));
		GetContext(ren)->Unmap(constant_buffer, 0u);
	}

	void Bind(Render& ren) override
	{
		switch (type)
		{
		case UNDEFINED:
		{
			//custom_exception error("Render Error", "Undefined Constant Buffer type");
			//throw error;
		}
			break;
		case VERTEX_SHADER_BUFFER:
			GetContext(ren)->VSSetConstantBuffers(0u, 1u, &constant_buffer);
			break;
		case PIXEL_SHADER_BUFFER:
			GetContext(ren)->PSSetConstantBuffers(0u, 1u, &constant_buffer);
			break;
		}
	}

protected:
	ID3D11Buffer* constant_buffer = nullptr;
	BUFFER_TYPE type = UNDEFINED;
};


