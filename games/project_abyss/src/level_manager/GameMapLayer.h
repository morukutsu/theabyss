// project_abyss
// LittleSpace Studio 2012

#ifndef MK_GAMEMAPLAYER
#define MK_GAMEMAPLAYER

#include "mkengine.h"

class GameMap;
class GameMapLayer : public mk::Drawable
{
	public:
		GameMapLayer(GameMap* map, int layerID, bool opaque);
		~GameMapLayer();
		void Draw();
		void Interpolate(float dt);

	public:
		GameMap* map;
		int layerID;
};

#endif