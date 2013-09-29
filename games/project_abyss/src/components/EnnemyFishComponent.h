// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENNEMY_FISH_COMPONENT
#define MK_ENNEMY_FISH_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

#define ENNEMY_FISH_ATTACK_RADIUS	600.0f

class AnimatedGraphicsComponent;
class IABaseEnnemyComponent;
class GraphicsComponent;
class BodyComponent;
class LightComponent;
class EnnemyFishComponent : public Component
{
	public:
		EnnemyFishComponent();
		~EnnemyFishComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void LookForPlayer();
		void CheckCollisionsWithBullets();

		void HitFeedback(NVector disp);

	public:
		IABaseEnnemyComponent* cmp;
		AnimatedGraphicsComponent *gfx;
		BodyComponent* body;
		LightComponent* light;

		// Etat
		int state, prevState;
		int life;

		float deadTime;
		float progressiveSpeed, targetSpeed;

		// Déplacement
		NVector moveVel;

		bool mirrorH;

		// Hit
		NVector feedbackVec;
		bool lockMirror;
};

#endif