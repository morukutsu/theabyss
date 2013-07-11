// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENNEMY_FISH_COMPONENT
#define MK_ENNEMY_FISH_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

#define ENNEMY_FISH_ATTACK_RADIUS	400.0f

class IABaseEnnemyComponent;
class GraphicsComponent;
class BodyComponent;
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

	public:
		IABaseEnnemyComponent* cmp;
		GraphicsComponent* gfx;
		BodyComponent* body;

		// Etat
		int state, prevState;
		int life;

		float deadTime;

		// Déplacement
		NVector moveVel;
};

#endif