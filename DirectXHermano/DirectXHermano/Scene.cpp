#include "Scene.h"
#include "Entity.h"
#include "ImGui/imgui.h"

Scene::~Scene()
{
	selected_entity = nullptr;

	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
		entities[i] = nullptr;
	}

	entities.erase(entities.begin(), entities.end());
}

void Scene::Update()
{
	//Delete entities
	DeleteEntities();

	//Update eNTITIES
	UpdateEntities();
}

void Scene::Draw(Render & ren)
{
	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); iter++)
	{
		(*iter)->Draw(ren);
	}
}

Entity& Scene::AddEntity()
{
	Entity* new_entity = new Entity(entities.size());
	entities.push_back(new_entity);

	return (*new_entity);
}

void Scene::DeleteEntity(Entity& entity_to_delete)
{
	entity_to_delete.Delete();
}

void Scene::DrawUI(ResourceManager& res)
{
	ImGui::Begin("Scene Window");

	if (ImGui::Button("Add Entity"))
	{
		AddEntity();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Entity"))
	{
		if (selected_entity)
		{
			DeleteEntity((*selected_entity));
			selected_entity = nullptr;
		}
	}

	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); iter++)
	{
		if (ImGui::Selectable((*iter)->GetName(), (*iter)->IsSelected()))
		{
			if (selected_entity)
				selected_entity->SetSelected(false);

			selected_entity = (*iter);
			selected_entity->SetSelected(true);
		}
	}

	ImGui::Begin("Inspector");
	if (selected_entity)
		selected_entity->DrawUI(res);
	ImGui::End();


	ImGui::End();
}

void Scene::DeleteEntities()
{
	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); )
	{
		if ((*iter)->IsDeleted())
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = entities.erase(iter);
			continue;
		}

		iter++;
	}
}

void Scene::UpdateEntities()
{
	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); iter++)
	{
		(*iter)->Update();

		if ((*iter)->IsLight())
		{
			(*iter)->AddLight();
			lights.push_back((*iter));
		}
	}
}