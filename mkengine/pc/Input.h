// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_INPUT
#define MK_INPUT

#include "lowSystem.h"

#define BUT_NONE          -1
#define BUT_PUSHED         0
#define BUT_HELD           1
#define BUT_RELEASED       2

#define CNT_KEYBOARD	   0
#define CNT_XBOX     	   1
#define CNT_GAMEPAD        2

#define MAX_BUTTONS		   18
#define MAX_STICKS		   2
#define MAX_PLAYERS        4

// Input : platform specific controller input handling
namespace mk 
{
	struct ButtonNames {
		enum ButtonName {
			X, Y, A, B, Back, Start, LB, LT, RB, RT, LeftStick, RightStick, Up, Down, Left, Right, LeftClick, RightClick
		};
	};
	typedef ButtonNames::ButtonName ButtonName;

	struct StickNames {
		enum StickName {
			Left, Right
		};
	};
	typedef StickNames::StickName StickName;

	struct Button
	{
		bool held, pressed, released;
	};

	struct Stick 
	{
		float magnitude;
		float angle;
	};

	struct Pointer
	{
		float x, y;
	};

	// Input pour un controleur
	struct Input
    {
		// Pad buttons
		Button buttons[MAX_BUTTONS];

		// Sticks
		Stick sticks[MAX_STICKS];

		// Pointer
		Pointer pointer;
	};

	class InputManager
	{
		public:
			static void Init();
			static void Update();
			static Input* GetInput(int player, int device);

		private:
			static Input _in[MAX_PLAYERS];
			static void majPressedReleased(bool oldheld, bool held, bool * pressed, bool * released)
			{
				if(!oldheld && held)
					*pressed = true;
				if(oldheld && !held)
					*released = true;
			}

			static void ResetInput(Input* input) {
				for(int k = 0; k < MAX_BUTTONS; k++) {
					input->buttons[k].held = false;
					input->buttons[k].pressed = false;
					input->buttons[k].released = false;
				}
			}
	};
};

#endif