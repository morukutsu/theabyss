// project_abyss
// LittleSpace Studio 2012

#include "EntityManager.h"
#include "Entity.h"
#include "Messages.h"
#include <string>

EntityManager::EntityManager()
{
	mNextAvailableId = 0;
	InitCommonVariables();
	bulletManager.SetEntityManager(this);
	entitiesHashMap.clear();
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
	//commonStateVariables[C_STATE_PLAYER_MIRROR] = 0;
}

void EntityManager::Add(Entity* ent)
{
	ent->SetId(mNextAvailableId);
	ent->SetEntityManager(this);
	entities.push_back(ent);
		
	mNextAvailableId++;

	// Add entity to the hashmap
	//entitiesHashMap[ent->mName] = ent;
	entitiesHashMap.insert(std::pair<std::string, Entity*>(ent->mName, ent));
}

void EntityManager::Update()
{
	// Update all the entities
	for(std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++) 
	{
		(*it)->Update();
	}

	// Update bullet manager
	bulletManager.Update();
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

Entity* EntityManager::GetEntityByName(std::string name) 
{ 
	std::map<std::string, Entity*>::iterator it = entitiesHashMap.find(name);
	if(it != entitiesHashMap.end() ) 
	{
		return entitiesHashMap.find(name)->second;
	}

	return NULL;
}

void EntityManager::SendMessageToEntity(std::string entityName, int message, void* data)
{
	Entity* ent = GetEntityByName(entityName);
	if(ent)
	{
		ent->Send(message, data);
	}
}

// 0 si pas de collision, 1 si oui, 2 la collision vient de se produire
int EntityManager::checkCollisionBetweenEntities(std::string &ent1, std::string &ent2)
{
	// Récupère les deux entités en fct de leur noms
	Entity* a = GetEntityByName(ent1);
	Entity* b = GetEntityByName(ent2);

	if(a == NULL || b == NULL)
		return -1;

	// Récupère les deux bodies parametrés pour cette collision
	CBody* body_1 = a->sensorBody;
	CBody* body_2 = b->sensorBody;

	if(body_1 == NULL || body_2 == NULL)
		return -1;

	// Check si ils sont en collision
	if(body_1->isCollision && body_2->isCollision) 
	{
		// Check s'ils sont en collision entre deux
		std::vector<CBody*> c = body_1->GetCollisionsBodies();
		for(int i = 0; i < c.size(); i++) 
		{
			// Il y a collision entre les deux body
			if(body_2 == c[i])
			{
				return 1;
			}
		}
	}

	return 0;
}

void EntityManager::setEntityPosition(std::string& name, float x, float y)
{
	Entity* a = GetEntityByName(name);
	if(a == NULL)
		lowError("Entity " + name + "does not exist");
	a->mPos.x = x;
	a->mPos.y = y;
}

float EntityManager::getEntityPositionX(std::string& name)
{
	Entity* a = GetEntityByName(name);
	if(a == NULL)
		lowError("Entity " + name + "does not exist");
	return a->mPos.x;
}

float EntityManager::getEntityPositionY(std::string& name)
{
	Entity* a = GetEntityByName(name);
	if(a == NULL)
		lowError("Entity " + name + "does not exist");
	return a->mPos.y;
}

void EntityManager::addWaypointToEntity(std::string& ent_name, std::string& waypt_name)
{
	// Récupère les deux entités en fct de leur noms
	Entity* ent = GetEntityByName(ent_name);
	Entity* waypoint = GetEntityByName(waypt_name);

	if(ent == NULL || waypoint == NULL)
		return;

	ent->AddWaypoint(waypoint);
}