#pragma once
#include "Bindable.h"
#include "Resource.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Render& ren, LPCWSTR name)
	{
		if (FAILED(D3DReadFileToBlob(name, &byte_code)))
		{
			//custom_exception error("RENDER ERROR", "VERTEX SHADER FAILED TO LOAD");
			//throw error;
		}

		GetDevice(ren)->CreateVertexShader(byte_code->GetBufferPointer(), byte_code->GetBufferSize(), nullptr, &vertex_shader);
		
	}
	~VertexShader()
	{
		if (vertex_shader)
			vertex_shader->Release();

		if (byte_code)
			byte_code->Release();
	}

	void Bind(Render& ren) override
	{
		if(vertex_shader)
			GetContext(ren)->VSSetShader(vertex_shader, nullptr, 0u);
	}

	ID3DBlob* GetByteCode() const { return byte_code; }

private:
	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* byte_code = nullptr;
};

class PixelShader : public Bindable
{
public:
	PixelShader(Render& ren, LPCWSTR name)
	{
		if (FAILED(D3DReadFileToBlob(name, &byte_code)))
		{
			//custom_exception error("RENDER ERROR", "PIXEL SHADER FAILED TO LOAD");
			//throw error;
		}

		GetDevice(ren)->CreatePixelShader(byte_code->GetBufferPointer(), byte_code->GetBufferSize(), nullptr, &pixel_shader);
	}
	~PixelShader()
	{
		if (pixel_shader)
			pixel_shader->Release();

		if (byte_code)
			byte_code->Release();
	}

	void Bind(Render& ren) override
	{
		if (pixel_shader)
			GetContext(ren)->PSSetShader(pixel_shader, nullptr, 0u);
	}

	ID3DBlob* GetByteCode() const { return byte_code; }

private:
	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* byte_code = nullptr;
};

class ShaderProgram : public Bindable, public Resource
{
public:
	ShaderProgram(Render& ren, LPCWSTR vertex_name, LPCWSTR pixel_name) : Resource(RESOURCE_TYPE::SHADER)
	{
		vertex_shader = new VertexShader(ren, vertex_name);
		pixel_shader = new PixelShader(ren, pixel_name);
	}
	~ShaderProgram()
	{
		if (vertex_shader)
		{
			delete vertex_shader;
			vertex_shader = nullptr;
		}
		if (pixel_shader)
		{
			delete pixel_shader;
			pixel_shader = nullptr;
		}
	}

	void Bind(Render& ren) override
	{
		if (vertex_shader && pixel_shader)
		{
			vertex_shader->Bind(ren);
			pixel_shader->Bind(ren);
		}
		else
		{
			//custom_exception error("RENDER ERROR", "SHADER PROGRAM MISS ONE SHADER TO LOAD");
			//throw error;
		}

	}

	ID3DBlob* GetVertexByteCode() const { return vertex_shader->GetByteCode(); }
	ID3DBlob* GetPixelByteCode() const { return pixel_shader->GetByteCode(); }

private:
	VertexShader* vertex_shader = nullptr;
	PixelShader*  pixel_shader = nullptr;
};