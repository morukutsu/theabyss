// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Input.h"
#include "mkengine.h"

namespace mk 
{
	Input InputManager::_in[MAX_PLAYERS];
	StringBimap<sf::Keyboard::Key> *InputManager::mKeybMap;
	sf::Keyboard::Key InputManager::mKeybMapping[MAX_PLAYERS][MAX_BUTTONS];

	void InputManager::Init()
	{
		InitMaps(); 

		// Initialisation mapping de base des touches clavier
		for(int i = 0; i < MAX_BUTTONS; i++)
		{
			for(int k = 0; k < MAX_PLAYERS; k++)
				mKeybMapping[k][i] = sf::Keyboard::A;
		}
	}

	void InputManager::Update()
	{
		//Sur PC, soit une utilise une vraie wiimote... soit le clavier/gamepad
		Input old = _in[0];
		Input in;
		ResetInput(&in);

		//Mise à jour états normaux

		// Keyboard
		for(int i = 0; i < MAX_BUTTONS; i++)
		{
			in.buttons[i].held = sf::Keyboard::isKeyPressed(mKeybMapping[0][i]);
		}
		
		/*in.buttons[ButtonNames::B].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		in.buttons[ButtonNames::X].held = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		in.buttons[ButtonNames::Y].held = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

		in.buttons[ButtonNames::Up].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);	
		in.buttons[ButtonNames::Left].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);	
		in.buttons[ButtonNames::Right].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);	
		in.buttons[ButtonNames::Down].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);	*/

		// On verra plus tard pour la souris
		in.buttons[ButtonNames::LeftClick].held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);	
		in.buttons[ButtonNames::RightClick].held = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);	

		//Mise a jour états pressed/released
		for(int k = 0; k < MAX_BUTTONS; k++) {
			majPressedReleased(old.buttons[k].held, in.buttons[k].held, &in.buttons[k].pressed, &in.buttons[k].released);
		}
		
		// Pointer
		// get global mouse position
		sf::Vector2i position = sf::Mouse::getPosition(*App);

		// Correction de la position du pointeur en fonction du scaling de la fenetre
		in.pointer.x = position.x * (1.0f/mk::Core::GetScaleFactor());
		in.pointer.y = position.y * (1.0f/mk::Core::GetScaleFactor());

		// Emulation du stick (émulation avec les touches de direction)
		if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_UP]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 359;			
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_RIGHT]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 90;
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_DOWN]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 180;
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_LEFT]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 270;

		if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_UP]) && sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_RIGHT]))
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 45;
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_DOWN]) && sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_RIGHT]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 135;
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_DOWN]) && sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_LEFT]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 225;
		else if(sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_UP]) && sf::Keyboard::isKeyPressed(in.emuKeyJoy[EMU_KEY_JOY_LEFT]) )
			in.sticks[StickName::Right].magnitude = 1.0f, in.sticks[StickName::Right].angle = 315;

		_in[0] = in;
	}

	Input* InputManager::GetInput(int player, int device)
	{
		return &_in[player];
	}

	void InputManager::InitMaps()
	{
		mKeybMap = new StringBimap<sf::Keyboard::Key>(sf::Keyboard::KeyCount, 1);

		#define THOR_INSERT_MAPPING(identifier) mKeybMap->insert(sf::Keyboard::identifier, #identifier)

		THOR_INSERT_MAPPING(Unknown);
		THOR_INSERT_MAPPING(A);
		THOR_INSERT_MAPPING(B);
		THOR_INSERT_MAPPING(C);
		THOR_INSERT_MAPPING(D);
		THOR_INSERT_MAPPING(E);
		THOR_INSERT_MAPPING(F);
		THOR_INSERT_MAPPING(G);
		THOR_INSERT_MAPPING(H);
		THOR_INSERT_MAPPING(I);
		THOR_INSERT_MAPPING(J);
		THOR_INSERT_MAPPING(K);
		THOR_INSERT_MAPPING(L);
		THOR_INSERT_MAPPING(M);
		THOR_INSERT_MAPPING(N);
		THOR_INSERT_MAPPING(O);
		THOR_INSERT_MAPPING(P);
		THOR_INSERT_MAPPING(Q);
		THOR_INSERT_MAPPING(R);
		THOR_INSERT_MAPPING(S);
		THOR_INSERT_MAPPING(T);
		THOR_INSERT_MAPPING(U);
		THOR_INSERT_MAPPING(V);
		THOR_INSERT_MAPPING(W);
		THOR_INSERT_MAPPING(X);
		THOR_INSERT_MAPPING(Y);
		THOR_INSERT_MAPPING(Z);
		THOR_INSERT_MAPPING(Num0);
		THOR_INSERT_MAPPING(Num1);
		THOR_INSERT_MAPPING(Num2);
		THOR_INSERT_MAPPING(Num3);
		THOR_INSERT_MAPPING(Num4);
		THOR_INSERT_MAPPING(Num5);
		THOR_INSERT_MAPPING(Num6);
		THOR_INSERT_MAPPING(Num7);
		THOR_INSERT_MAPPING(Num8);
		THOR_INSERT_MAPPING(Num9);
		THOR_INSERT_MAPPING(Escape);
		THOR_INSERT_MAPPING(LControl);
		THOR_INSERT_MAPPING(LShift);
		THOR_INSERT_MAPPING(LAlt);
		THOR_INSERT_MAPPING(LSystem);
		THOR_INSERT_MAPPING(RControl);
		THOR_INSERT_MAPPING(RShift);
		THOR_INSERT_MAPPING(RAlt);
		THOR_INSERT_MAPPING(RSystem);
		THOR_INSERT_MAPPING(Menu);
		THOR_INSERT_MAPPING(LBracket);
		THOR_INSERT_MAPPING(RBracket);
		THOR_INSERT_MAPPING(SemiColon);
		THOR_INSERT_MAPPING(Comma);
		THOR_INSERT_MAPPING(Period);
		THOR_INSERT_MAPPING(Quote);
		THOR_INSERT_MAPPING(Slash);
		THOR_INSERT_MAPPING(BackSlash);
		THOR_INSERT_MAPPING(Tilde);
		THOR_INSERT_MAPPING(Equal);
		THOR_INSERT_MAPPING(Dash);
		THOR_INSERT_MAPPING(Space);
		THOR_INSERT_MAPPING(Return);
		THOR_INSERT_MAPPING(BackSpace);
		THOR_INSERT_MAPPING(Tab);
		THOR_INSERT_MAPPING(PageUp);
		THOR_INSERT_MAPPING(PageDown);
		THOR_INSERT_MAPPING(End);
		THOR_INSERT_MAPPING(Home);
		THOR_INSERT_MAPPING(Insert);
		THOR_INSERT_MAPPING(Delete);
		THOR_INSERT_MAPPING(Add);
		THOR_INSERT_MAPPING(Subtract);
		THOR_INSERT_MAPPING(Multiply);
		THOR_INSERT_MAPPING(Divide);
		THOR_INSERT_MAPPING(Left);
		THOR_INSERT_MAPPING(Right);
		THOR_INSERT_MAPPING(Up);
		THOR_INSERT_MAPPING(Down);
		THOR_INSERT_MAPPING(Numpad0);
		THOR_INSERT_MAPPING(Numpad1);
		THOR_INSERT_MAPPING(Numpad2);
		THOR_INSERT_MAPPING(Numpad3);
		THOR_INSERT_MAPPING(Numpad4);
		THOR_INSERT_MAPPING(Numpad5);
		THOR_INSERT_MAPPING(Numpad6);
		THOR_INSERT_MAPPING(Numpad7);
		THOR_INSERT_MAPPING(Numpad8);
		THOR_INSERT_MAPPING(Numpad9);
		THOR_INSERT_MAPPING(F1);
		THOR_INSERT_MAPPING(F2);
		THOR_INSERT_MAPPING(F3);
		THOR_INSERT_MAPPING(F4);
		THOR_INSERT_MAPPING(F5);
		THOR_INSERT_MAPPING(F6);
		THOR_INSERT_MAPPING(F7);
		THOR_INSERT_MAPPING(F8);
		THOR_INSERT_MAPPING(F9);
		THOR_INSERT_MAPPING(F10);
		THOR_INSERT_MAPPING(F11);
		THOR_INSERT_MAPPING(F12);
		THOR_INSERT_MAPPING(F13);
		THOR_INSERT_MAPPING(F14);
		THOR_INSERT_MAPPING(F15);
		THOR_INSERT_MAPPING(Pause);

		#undef THOR_INSERT_MAPPING
	}
};