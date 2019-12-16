#pragma once
#include <vector>

class Render;
class Entity;
class ResourceManager;

class Scene
{
public:
	Scene() = default;
	~Scene();

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Update();
	void Draw(Render& ren);
	void DrawLights(Render& ren);
	void DrawLightAt(int position, Render& ren);

	Entity& AddEntity();
	void DeleteEntity(Entity&);

	void DrawUI(ResourceManager&);

	int GetNumLights() const { return lights.size(); }
	int GetLightType(int light) const;
private:
	Entity* selected_entity = nullptr;

	std::vector<Entity*> entities;
	std::vector<Entity*> lights;

	void DeleteEntities();
	void UpdateEntities();
};