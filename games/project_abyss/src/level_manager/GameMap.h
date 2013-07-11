// project_abyss
// LittleSpace Studio 2012

// Gestion des maps ainsi que de toutes les entités
#ifndef MK_GAMEMAP
#define MK_GAMEMAP

#define GRAVITY		800.0f

#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

class Entity;
class EntityManager;
class GameMapLayer;
class CBody;
class LevelManager;
class GameMap : public mk::Map
{
public:
	GameMap(LevelManager* lvl);
	~GameMap();

	void Init();
	void LoadGameMap(mk::Scene* scene);
	void AddMapLayersToScene(mk::Scene* scene);
	EntityManager* GetEntityManager() { return entityManager; };
	void SpecialCases(std::string name, std::string type, Entity* entity);
	void CreateHeroEntity();

	void UpdatePhysics();

	void AddBody(CBody* body);
	void RemoveBody(CBody* body);

	void DebugCollisions();

	std::string GetScriptFilename() { return scriptFilename; }
	
	LevelManager* GetLevelManager() { return lvlMan; };

public:
	// Entités
	EntityManager* entityManager;

	// Cas particuliers d'entités
	Entity* heroEntity, *camEntity;

	// Gestion scrolling
	float borderX, borderY, borderW, borderH;

	// Collisions
	std::list<CBody*> bodies;

	// Positionnement du héros au départ
	float startHeroX, startHeroY;
	std::string mSwitchMapDoorIdf;

	std::list<GameMapLayer*> layersDrawables;

	// Script
	std::string scriptFilename;

	// LevelManager
	LevelManager* lvlMan;

};

#endif