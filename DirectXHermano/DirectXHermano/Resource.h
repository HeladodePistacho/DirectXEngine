#pragma once
#include <string>

enum RESOURCE_TYPE
{
	RESOURCE_UNDEFINED = 0,
	MESH,
	TEXTURE,
	SHADER,
	MATERIAL
};

class Resource
{
public:
	Resource(RESOURCE_TYPE _type, std::string _path) : type(_type), path(_path) 
	{
		if(path.find_last_of('/') != -1)
			name = path.substr(path.find_last_of('/') + 1);	
		else name = path.substr(path.find_last_of('\\') + 1);
	}

	RESOURCE_TYPE GetType() const { return type; }

	const char* GetName() const { return name.c_str(); }
private:
	RESOURCE_TYPE type = RESOURCE_UNDEFINED;
	bool needs_load = true;

	std::string name;
	std::string path;
};
