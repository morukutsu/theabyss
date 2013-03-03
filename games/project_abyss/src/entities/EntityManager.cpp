// project_abyss
// LittleSpace Studio 2012

#include "EntityManager.h"
#include "Entity.h"
#include "Messages.h"

EntityManager::EntityManager()
{
	mNextAvailableId = 0;
	InitCommonVariables();
	bulletManager.SetEntityManager(this);
}

EntityManager::~EntityManager()
{
	for(std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ) 
	{
		delete (*it);
		entities.erase(it++);
	}
}

void EntityManager::InitCommonVariables()
{
	commonStateVariables[C_STATE_GOING_TROUGH_DOOR] = 0;
	commonStateVariables[C_STATE_EXITING_DOOR] = 0;
	commonStateVariables[C_STATE_LOCK_USER_INPUT] = 0;
}

void EntityManager::Add(Entity* ent)
{
	ent->SetId(mNextAvailableId);
	ent->SetEntityManager(this);
	entities.push_back(ent);
	
	mNextAvailableId++;
}

void EntityManager::Update()
{
	// Update all the entities
	for(std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++) 
	{
		(*it)->Update();
	}
}

void EntityManager::Receive(int message, void* data)
{
	// Recoit un message envoyé depuis une entité
	std::cout << "[Messages] EntityManager received code " << message << std::endl;

	switch(message)
	{
	case MSG_CHANGE_STATE_VAR:
		MsgDataChangeStateVar* v = (MsgDataChangeStateVar*)data;
		commonStateVariables[v->idf] = v->value;
		break;
	};
}
