// project_abyss
// LittleSpace Studio 2012

#ifndef MK_DOOR_COMPONENT
#define MK_DOOR_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include "../entities/Messages.h"

enum {
	DOOR_LEFT, DOOR_RIGHT, DOOR_UP, DOOR_DOWN
};

enum {
	DOOR_STATE_CLOSED, DOOR_STATE_OPENED, DOOR_STATE_SCROLLING
};

class CBody;
class DoorComponent : public Component
{
	public:
		DoorComponent(std::string _sens, float w, float h, std::string _target_map, std::string _target_door);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		int sens;
		float width, height;
		CBody* sensor;

		// Etats de la porte
		int state;
		std::string target_map, target_door;
};

#endif
