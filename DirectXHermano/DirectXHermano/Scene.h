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

	void SetSelectedEntity(Entity& entity);
	Entity& AddEntity();
	void DeleteEntity(Entity&);

	void DrawUI(ResourceManager&);
private:
	Entity* selected_entity = nullptr;
	std::vector<Entity*> entities;

	void DeleteEntities();
	void UpdateEntities();
};