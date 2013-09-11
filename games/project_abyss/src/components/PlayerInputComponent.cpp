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
	/*if(input->buttons[mk::ButtonNames::RT].pressed)
		PushCommand(CMD_SHOOT);*/

	if(input->buttons[mk::ButtonNames::LeftClick].held)
		PushCommand(CMD_SHOOT);

	// Orientation du canon
	if(input->sticks[mk::StickName::Right].magnitude > 0.5f)
	{
		if(input->sticks[mk::StickName::Right].angle > 360 - 20 || input->sticks[mk::StickName::Right].angle < 0 + 20)
			PushCommand(CMD_ORIENT_UP);
		else if(input->sticks[mk::StickName::Right].angle > 180 - 20 && input->sticks[mk::StickName::Right].angle < 180 + 20)
			PushCommand(CMD_ORIENT_DOWN);
		else if((input->sticks[mk::StickName::Right].angle > 45 - 20 && input->sticks[mk::StickName::Right].angle < 45 + 20)
			|| (input->sticks[mk::StickName::Right].angle > 315 - 20 && input->sticks[mk::StickName::Right].angle < 315 + 20))
			PushCommand(CMD_ORIENT_UP_DIAG);
		else if((input->sticks[mk::StickName::Right].angle > 225 - 20 && input->sticks[mk::StickName::Right].angle < 225 + 20) 
			|| (input->sticks[mk::StickName::Right].angle > 135 - 20 && input->sticks[mk::StickName::Right].angle < 135 + 20))
			PushCommand(CMD_ORIENT_DOWN_DIAG);
		else
			PushCommand(CMD_ORIENT_SIDE);	
	
	
	}
	else
	{
		// De base le canon est orienté sur les côtés
		PushCommand(CMD_ORIENT_SIDE);
	}
}

void PlayerInputComponent::PushCommand(int command)
{
	commands.push_back(command);
}

void PlayerInputComponent::Receive(int message, void* data)
{

}
