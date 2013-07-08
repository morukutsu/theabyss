// project_abyss
// LittleSpace Studio 2012

#ifndef MK_IA_BASE_ENNEMY_COMPONENT
#define MK_IA_BASE_ENNEMY_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

class IABaseEnnemyComponent : public Component
{
	public: 
	enum {
		S_WAIT, S_MOVE, S_ATTACK, S_HIT, S_DEAD
	};

	public:
		IABaseEnnemyComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void Automate();
		void ChangeState(int st) { previousState = currentState; currentState = st; stateTime = 0.0f; };

		void SetLife(int l) { life = l; };
		int GetLife() { return life; };

		void SetPlayerSpotted(bool v) { playerSpotted = v; };
		bool GetPlayerSpotted() { return playerSpotted; };

		void SetPlayerTouched(bool v) { playerTouched = v; };
		bool GetPlayerTouched() { return playerTouched; };

		int  GetState() { return currentState; };
		int	 GetPrevState() { return previousState; };

	private:
		int currentState, previousState;

		// Variables de l'automate
		float stateTime;
		bool playerSpotted;
		int life;
		bool playerTouched;
};

#endif