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
#define ENN_BIGFISH_SPOTTED_RADIUS  650.0f
#define ENN_BIGFISH_ATTACK_RADIUS	400.0f
#define ENN_BIGFISH_KILL_RADIUS		125.0f

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

		void Move(float speed);
		bool WaypointReached();

		void LaunchGameoverScene();

		void CheckCollisions();

	public:
		AnimatedGraphicsComponent *gfx;
		GraphicsComponent *flare, *halo;

		BodyComponent* body;
		LightComponent *light, *lightEye;

		int boneID, boneID_eye;

		// Les deux textures
		mk::Image *texA, *texB;

		// Waypoints
		Entity* nextEntityToFollow;
		int curWaypointID;
		NVector vel;
		bool mirrorH;
		bool lockMirror;
		bool isSpotted;

		// IA Statemachine
		int state;
		float mWaitTime, mSpottedWaitTime, mAttackSpeed;

		// Cutscene de fin
		int cutsceneState;
		
};

#endif
