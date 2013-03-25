// project_abyss
// LittleSpace Studio 2012

#include "PlayerInputComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

PlayerInputComponent::PlayerInputComponent()
{

}

void PlayerInputComponent::Init()
{
	
}


void PlayerInputComponent::Update()
{
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);
	commands.clear();

	// Déplacement du vaisseau
	if(input->buttons[mk::ButtonNames::Up].held)
		PushCommand(CMD_ACCEL_UP);
	if(input->buttons[mk::ButtonNames::Down].held)
		PushCommand(CMD_ACCEL_DOWN);
	if(input->buttons[mk::ButtonNames::Left].held)
		PushCommand(CMD_ACCEL_LEFT);
	if(input->buttons[mk::ButtonNames::Right].held)
		PushCommand(CMD_ACCEL_RIGHT);

	// Tir
	if(input->buttons[mk::ButtonNames::RT].pressed)
		PushCommand(CMD_SHOOT);
}

void PlayerInputComponent::PushCommand(int command)
{
	commands.push_back(command);
}

void PlayerInputComponent::Receive(int message, void* data)
{

}
