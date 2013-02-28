// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENTITYMGR
#define MK_ENTITYMGR

#include <list>
#include "mkengine.h"

#define COMMON_STATE_VAR_SIZE 16

enum {
	C_STATE_GOING_TROUGH_DOOR, C_STATE_DOOR_ORIENTATION, C_STATE_EXITING_DOOR,
	C_STATE_LOCK_USER_INPUT
};

class Entity;
class GameMap;
class EntityManager
{
	public:
		EntityManager();
		~EntityManager();

		void InitCommonVariables();

		void Add(Entity* ent);
		void Update();
		
		int  GetEntitiesCount() { return entities.size(); };
		void SetScene(mk::Scene* scene) { parentScene = scene; };

		void SetGameMap(GameMap* map) { parentMap = map; };
		GameMap* GetGameMap() { return parentMap; };

		void Receive(int message, void* data);

		int* GetCommonStateVariables() { return commonStateVariables; };

	private:
		// Entit�s
		std::list<Entity*> entities;
		int mNextAvailableId;

		// pointeur sur la scene parente
		mk::Scene* parentScene;
		GameMap* parentMap;

		// Variables d'�tats partag�es
		int commonStateVariables[COMMON_STATE_VAR_SIZE];
	
	public:
		// Changement de map
		std::string targetMap, targetDoor;
};

#endif