// project_abyss
// LittleSpace Studio 2012

#include "PlayerMovementComponent.h"
#include "../entities/Entity.h"
#include "PlayerInputComponent.h"
#include "PlayerBodyComponent.h"
#include "../physics/Body.h"
#include "../entities/EntityManager.h"
#include "DoorComponent.h"

PlayerMovementComponent::PlayerMovementComponent(PlayerInputComponent* in, PlayerBodyComponent* bod)
{
	input = in;
	body = bod;

	// Constantes du jeu
	MAX_VELOCITY = 15.0f;
	ACCELERATION = 2000.0f;
	DECELERATION = 0.9f;

	isGoingThroughDoor = false;
}

void PlayerMovementComponent::Init()
{
	parent->mVel = NVector(0, 0);
}

void PlayerMovementComponent::Receive(int message, void* data)
{

}

void PlayerMovementComponent::Update()
{
	// Lock de l'user input
	if(parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_LOCK_USER_INPUT] == 1)
		input->commands.clear();

	// Gestion des déplacements dans les portes
	HandleDoors();

	// Application des déplacements
	ProcessCommands();

	// Setting de la positon depuis le moteur physique
	parent->mPos = body->cockpit->GetPosition();

	// Déceleration TODO : à voir comment mieux coder ça ...
	body->cockpit->m_xDisplacement *= DECELERATION;

	parent->mVel = body->cockpit->GetDisplacement();
}

void PlayerMovementComponent::ProcessCommands()
{
	// Remise a zero des etats
	isPlayerAccelerated = false;

	// Traite toutes les commandes reçues depuis l'input à cette frame
	for(std::list<int>::iterator it = input->commands.begin(); it != input->commands.end(); it++)
	{
		int command = (*it);
		switch(command)
		{
			case CMD_ACCEL_UP:
			case CMD_ACCEL_DOWN:
			case CMD_ACCEL_LEFT:
			case CMD_ACCEL_RIGHT:
				HandleMovement(command);
			break;
		}
	}

	input->commands.clear();
}

void PlayerMovementComponent::HandleMovement(int command)
{
	// Gestion de l'accélération
	NVector curVel = NVector(0,0);
	if(command == CMD_ACCEL_UP)
	{
		curVel += NVector(0, -ACCELERATION);
	}
	else if(command == CMD_ACCEL_DOWN)
	{
		curVel += NVector(0, ACCELERATION);
	}
	else if(command == CMD_ACCEL_LEFT)
	{
		curVel += NVector(-ACCELERATION, 0);
	}
	else if(command == CMD_ACCEL_RIGHT)
	{
		curVel += NVector(ACCELERATION, 0);
	}

	isPlayerAccelerated = true;
	
	// Max speed
	if(parent->mVel.x < -MAX_VELOCITY && curVel.x < 0)
		curVel.x = 0;

	if(parent->mVel.x > MAX_VELOCITY && curVel.x > 0)
		curVel.x = 0;

	if(parent->mVel.y < -MAX_VELOCITY && curVel.y < 0)
		curVel.y = 0;

	if(parent->mVel.y > MAX_VELOCITY && curVel.y > 0)
		curVel.y = 0;

	body->cockpit->AddImpulse(curVel, 1.0f/30.0f);
}

void PlayerMovementComponent::HandleDoors()
{
	bool old = isGoingThroughDoor;
	isGoingThroughDoor = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_GOING_TROUGH_DOOR] == 1 
		|| parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] == 1;

	if((old != isGoingThroughDoor) && isGoingThroughDoor)
	{
		// Réinitialisation de la vitesse à zéro
		int sensPorte = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_DOOR_ORIENTATION];
		switch(sensPorte)
		{
			case DOOR_LEFT:
			case DOOR_RIGHT:
				body->cockpit->m_xDisplacement.y = 0;
				break;
			case DOOR_UP:
			case DOOR_DOWN:
				body->cockpit->m_xDisplacement.x = 0;
				break;
		};

		
	}

	if(!isGoingThroughDoor)
		return;

	// Selon l'orientation de la porte, on va dans une direction
	input->commands.clear();

	int sensPorte = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_DOOR_ORIENTATION];
	switch(sensPorte)
	{
		case DOOR_LEFT:
			input->commands.push_back(CMD_ACCEL_LEFT);
			break;
		case DOOR_RIGHT:
			input->commands.push_back(CMD_ACCEL_RIGHT);
			break;
		case DOOR_UP:
			input->commands.push_back(CMD_ACCEL_UP);
			break;
		case DOOR_DOWN:
			input->commands.push_back(CMD_ACCEL_DOWN);
			break;
	};
}

