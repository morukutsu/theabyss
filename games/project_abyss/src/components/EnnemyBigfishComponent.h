// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENNEMY_BIGFISH_COMPONENT
#define MK_ENNEMY_BIGFISH_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

#define ENN_BIGFISH_SPEED			4.0f
#define ENN_BIGFISH_SLOWDIST		100.0f
#define ENN_BIGFISH_WAIT  		    2.0f

class AnimatedGraphicsComponent;
class BodyComponent;
class LightComponent;
class GraphicsComponent;
class Entity;

class EnnemyBigfishComponent : public Component
{
	enum {
		S_SEEK, S_SLOW, S_WAIT, S_SPOTTED, S_ATTACK
	};

	public:
		EnnemyBigfishComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void FollowWaypoints();

	public:
		AnimatedGraphicsComponent *gfx;
		GraphicsComponent *flare, *halo;

		BodyComponent* body;
		LightComponent *light, *lightEye;

		int boneID, boneID_eye;

		// Waypoints
		Entity* nextEntityToFollow;
		int curWaypointID;
		NVector vel;
		bool mirrorH;
		bool lockMirror;

		// IA Statemachine
		int state;
		float mWaitTime;
};

#endif
