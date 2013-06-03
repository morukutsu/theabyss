// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENNEMY_FISH_COMPONENT
#define MK_ENNEMY_FISH_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include <list>
#include "../physics/Vector.h"

class IABaseEnnemyComponent;

class EnnemyFishComponent : public Component
{
	public:
		EnnemyFishComponent();
		~EnnemyFishComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		IABaseEnnemyComponent* cmp;
};

#endif