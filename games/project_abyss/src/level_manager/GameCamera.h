// project_abyssrage

// LittleSpace Studio 2012

#ifndef MK_GAMECAMERA
#define MK_GAMECAMERA

#include "mkengine.h"
#include "../physics/Vector.h"

class GameCamera
{
	public:
		GameCamera();

		void Reset();
		void Set();

		void MoveTo(float x, float y) { camX = x, camY = y; };
		void SetDepth(float z) { camZ = z; };

		float GetDepth() { return camZ; };
		float GetCamX() { return camX; };
		float GetCamY() { return camY; };
		
		float GetPixelPerfectPlane(mk::Map* map) { return 0.5f*mk::Core::getBaseHeight()/tan(DegreesToRadians(60/2))/map->GetMapScale(); };
		NVector Get3DPointScreenCoordinates(float x, float y, float z);
		NVector GetUnProject(float x, float y, float z);

		void DebugMove();
		void ShowDebugInfo(float x, float y);

	private:
		float camX, camY, camZ;
		mk::Font* debugFont;
};

#endif