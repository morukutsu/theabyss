// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PLAYER_INPUT_COMPONENT
#define MK_PLAYER_INPUT_COMPONENT

#include "Component.h"
#include "mkengine.h"

enum
{
	CMD_ACCEL_UP, CMD_ACCEL_DOWN, CMD_ACCEL_LEFT, CMD_ACCEL_RIGHT
};

class PlayerInputComponent : public Component
{
	public:
		PlayerInputComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void PushCommand(int command);

	public:
		std::list<int> commands;

};

#endif
