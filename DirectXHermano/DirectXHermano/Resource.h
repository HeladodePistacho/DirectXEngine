#pragma once

enum RESOURCE_TYPE
{
	UNDEFINED = 0,
	MESH,
	TEXTURE,
	SHADER
};

class Resource
{
public:
	Resource(RESOURCE_TYPE _type) : type(_type) {}

	RESOURCE_TYPE GetType() const { return type; }

private:
	RESOURCE_TYPE type = UNDEFINED;
	bool needs_load = true;
};
