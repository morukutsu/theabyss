// project_abyss
// LittleSpace Studio 2012

// Gestion des maps ainsi que de toutes les entités
#ifndef MK_GAMEMAP
#define MK_GAMEMAP

#include "mkengine.h"
#include <list>

class Entity;
class EntityManager;
class GameMapLayer;
class CBody;
class GameMap : public mk::Map
{
public:
	GameMap();
	~GameMap();

	void Init();
	void LoadGameMap(std::string filename, mk::Scene* scene);
	void AddMapLayersToScene(mk::Scene* scene);
	EntityManager* GetEntityManager() { return entityManager; };
	void SpecialCases(std::string name, std::string type, Entity* entity);
	void CreateHeroEntity();

	void UpdatePhysics();

	void AddBody(CBody* body);

	void DebugCollisions();

public:
	// Entités
	EntityManager* entityManager;

	// Cas particuliers d'entités
	Entity* heroEntity;

	// Gestion scrolling
	float borderX, borderY, borderW, borderH;

	// Collisions
	std::list<CBody*> bodies;

	// Positionnement du héros au départ
	float startHeroX, startHeroY;
	std::string mSwitchMapDoorIdf;

	std::list<GameMapLayer*> layersDrawables;
};

#endif