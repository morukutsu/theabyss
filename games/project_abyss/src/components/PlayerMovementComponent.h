// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PLAYER_MOVEMENT_COMPONENT
#define MK_PLAYER_MOVEMENT_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>

class PlayerInputComponent;
class PlayerBodyComponent;
class PlayerMovementComponent : public Component
{
	public:
		PlayerMovementComponent(PlayerInputComponent* in, PlayerBodyComponent* bod);

		void Receive(int message, void* data);

		void Update();

		void Init();

		void ProcessCommands();
		
		void HandleMovement(int command);

		void HandleDoors();

		void HandleCollisionwithEnnemies();

		void HandleMirrorH();

	public:
		PlayerInputComponent* input;
		PlayerBodyComponent* body;

		// Variables du gameplay
		float MAX_VELOCITY, ACCELERATION, DECELERATION, KNOCKBACK, KNOCKBACK_TIME;

		// Etats
		bool isPlayerAccelerated, isAcceleratedLeft, isAcceleratedRight;
		bool isGoingThroughDoor;
		bool isKnockback;

		// Gestion du knockback
		float mKnockbackTime;

		// Mirroring du vaisseau
		bool mirrorH, oldMirrorH;
};

#endif
