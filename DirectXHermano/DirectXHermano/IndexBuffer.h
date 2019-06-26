#pragma once
#include "Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public: 
	IndexBuffer(Render& ren, const std::vector<unsigned short>& indices);
	~IndexBuffer();

	void Bind(Render& ren) override;
	UINT GetNumIndices() { return num_indices; }
private:
	ID3D11Buffer* buffer_data = nullptr;
	UINT num_indices;
};