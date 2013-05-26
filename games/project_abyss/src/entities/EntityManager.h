// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENTITYMGR
#define MK_ENTITYMGR

#include <list>
#include "mkengine.h"

#include "../level_manager/BulletManager.h"

#include <map>
#include <string>
#include "../physics/Vector.h"

#define COMMON_STATE_VAR_SIZE 16

enum {
	C_STATE_GOING_TROUGH_DOOR, C_STATE_DOOR_ORIENTATION, C_STATE_EXITING_DOOR,
	C_STATE_LOCK_USER_INPUT, C_STATE_PLAYER_MOVED, C_STATE_PLAYER_MIRROR
};

class Entity;
class GameMap;
class EntityManager
{
	public:
		EntityManager();
		~EntityManager();

		void InitCommonVariables();

		void Add(Entity* ent, float lifetime = -1.0f);
		void Update();
		
		int  GetEntitiesCount() { return entities.size(); };
		void SetScene(mk::Scene* scene) { parentScene = scene; };

		void SetGameMap(GameMap* map) { parentMap = map; };
		GameMap* GetGameMap() { return parentMap; };
		BulletManager* GetBulletManager() { return &bulletManager; };

		void Receive(int message, void* data);

		int* GetCommonStateVariables() { return commonStateVariables; };
		mk::Scene* GetScene() { return parentScene; };

		Entity* GetEntityByName(std::string name);
		void SendMessageToEntity(std::string entityName, int message, void* data);

		NVector GetHeroPosition();

		// Pour les scripts
		int checkCollisionBetweenEntities(std::string& ent1, std::string& ent2);
		void setEntityPosition(std::string& name, float x, float y);
		float getEntityPositionX(std::string& name);
		float getEntityPositionY(std::string& name);
		void addWaypointToEntity(std::string& ent_name, std::string& waypt_name);

		// Particules
		void AddParticleImage(float x, float y, std::string filename, float lifetime);

	private:
		// Entit�s
		std::list<Entity*> entities;
		int mNextAvailableId;

		// Hash entities by names
		std::map<std::string, Entity*> entitiesHashMap;

		// Bullet Manager instance
		BulletManager bulletManager;

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