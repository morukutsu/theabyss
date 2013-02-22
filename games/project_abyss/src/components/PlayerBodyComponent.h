// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PLAYER_BODY_COMPONENT
#define MK_PLAYER_BODY_COMPONENT

#include "Component.h"
#include "mkengine.h"

class CBody;
class PlayerBodyComponent : public Component
{
	public:
		PlayerBodyComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		// Body, Hitboxes... composant le vaisseau
		CBody* cockpit;
};

#endif