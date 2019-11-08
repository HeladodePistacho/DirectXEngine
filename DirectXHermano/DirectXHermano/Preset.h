#pragma once
#include "Resource.h"
#include <vector>

class Preset : public Resource
{
public: 
	Preset(std::string name) : Resource(PRESET, name) {}
	~Preset() { materials_names.clear(); }

	void AddMaterialName(const char* name) { materials_names.push_back(name); }
	int GetNumMaterials() const { return materials_names.size(); }
	std::string GetMaterialAt(unsigned int index) const { return materials_names[index]; }
	
private:
	std::vector<std::string> materials_names;
};