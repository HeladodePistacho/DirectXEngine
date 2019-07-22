#pragma once

enum RESOURCE_TYPE
{
	RESOURCE_UNDEFINED = 0,
	MESH,
	TEXTURE,
	SHADER
};

class Resource
{
public:
	Resource(RESOURCE_TYPE _type, std::string _name) : type(_type), name(_name) {}

	RESOURCE_TYPE GetType() const { return type; }

	const char* GetName() const { return name.c_str(); }
private:
	RESOURCE_TYPE type = RESOURCE_UNDEFINED;
	bool needs_load = true;

	std::string name;
};
