// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Input.h"
#include "mkengine.h"

namespace mk 
{
	Input InputManager::_in[MAX_PLAYERS];

	void InputManager::Init()
	{
		
	}

	void InputManager::Update()
	{
		//Sur PC, soit une utilise une vraie wiimote... soit le clavier/gamepad
		Input old = _in[0];
		Input in;
		ResetInput(&in);

		//Mise à jour états normaux

		// Keyboard
		// TODO : Ajouter la modification du mapping des touches
		in.buttons[ButtonNames::A].held = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		in.buttons[ButtonNames::B].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
		in.buttons[ButtonNames::X].held = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
		in.buttons[ButtonNames::Y].held = sf::Keyboard::isKeyPressed(sf::Keyboard::R);

		in.buttons[ButtonNames::Up].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);	
		in.buttons[ButtonNames::Left].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);	
		in.buttons[ButtonNames::Right].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);	
		in.buttons[ButtonNames::Down].held = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);	

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

		//Nunchuk (émulation avec les touches de direction)
		/*if(Inputsf.IsKeyDown(sf::Key::Up) )
			in.magnitude = 1.0f, in.angle = 359;			
		else if(Inputsf.IsKeyDown(sf::Key::Right) )
			in.magnitude = 1.0f, in.angle = 90;
		else if(Inputsf.IsKeyDown(sf::Key::Down) )
			in.magnitude = 1.0f, in.angle = 180;
		else if(Inputsf.IsKeyDown(sf::Key::Left) )
			in.magnitude = 1.0f, in.angle = 270;

		if(Inputsf.IsKeyDown(sf::Key::Up) && Inputsf.IsKeyDown(sf::Key::Right))
			in.magnitude = 1.0f, in.angle = 45;
		else if(Inputsf.IsKeyDown(sf::Key::Down) && Inputsf.IsKeyDown(sf::Key::Right) )
			in.magnitude = 1.0f, in.angle = 135;
		else if(Inputsf.IsKeyDown(sf::Key::Down) && Inputsf.IsKeyDown(sf::Key::Left) )
			in.magnitude = 1.0f, in.angle = 225;
		else if(Inputsf.IsKeyDown(sf::Key::Up) && Inputsf.IsKeyDown(sf::Key::Left) )
			in.magnitude = 1.0f, in.angle = 315;*/

		_in[0] = in;
	}

	Input* InputManager::GetInput(int player, int device)
	{
		return &_in[player];
	}


};

