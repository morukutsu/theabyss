// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENNEMY_BIGFISH_COMPONENT
#define MK_ENNEMY_BIGFISH_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

#define ENN_BIGFISH_SPEED			4.0f

class AnimatedGraphicsComponent;
class BodyComponent;
class LightComponent;
class GraphicsComponent;
class Entity;
class EnnemyBigfishComponent : public Component
{
	public:
		EnnemyBigfishComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void FollowWaypoints();

	public:
		AnimatedGraphicsComponent* gfx;
		GraphicsComponent* halo;

		BodyComponent* body;
		LightComponent *light, *lightEye;

		int boneID, boneID_eye;

		// Waypoints
		Entity* nextEntityToFollow;
		int curWaypointID;
		NVector vel;
};

#endif
