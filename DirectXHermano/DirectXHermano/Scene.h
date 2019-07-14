#pragma once
#include <vector>

class Render;
class Entity;
class Scene
{
public:
	Scene() = default;
	~Scene();

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void Update();
	void Draw(Render& ren);

	void AddEntity();
	void DeleteEntity(Entity&);

private:
	Entity* selected_entity = nullptr;
	std::vector<Entity*> entities;

	void DeleteEntities();
	void UpdateEntities();
};