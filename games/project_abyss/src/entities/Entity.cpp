// project_abyss
// LittleSpace Studio 2012

#include "Entity.h"
#include "../components/Component.h"
#include "EntityManager.h"

Entity::~Entity()
{
	for(std::list<Component*>::iterator it = components.begin(); it != components.end(); )
	{
		delete (*it);
		components.erase(it++);
	}
}

void Entity::AddComponent(Component *comp)
{
	comp->SetParent(this);
	components.push_back(comp);
}

void Entity::Send(int message, void* data)
{
	for(std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Receive(message, data);
	}
}

void Entity::SendToManager(int message, void* data)
{
	entityManager->Receive(message, data);
}

void Entity::UpdateComponents()
{
	for(std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update();
	}
}

void Entity::InitComponents()
{
	for(std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Init();
	}
}