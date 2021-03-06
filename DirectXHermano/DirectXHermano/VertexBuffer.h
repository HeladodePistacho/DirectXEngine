#pragma once
#include "Bindable.h"
#include <vector>

struct Vertex
{
	struct
	{
		float x, y, z;
	} position, normal;	

	struct
	{
		float u, v;
	}texture_coords;
};

class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Render& ren, std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind(Render& ren) override;

private:
	ID3D11Buffer* buffer_data = nullptr;

};