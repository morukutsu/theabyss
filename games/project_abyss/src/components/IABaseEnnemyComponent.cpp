// project_abyss
// LittleSpace Studio 2012

#include "IABaseEnnemyComponent.h"

IABaseEnnemyComponent::IABaseEnnemyComponent()
{

}

void IABaseEnnemyComponent::Init()
{
	currentState = previousState = S_WAIT;
	stateTime = 0.0f;
	playerSpotted = playerTouched = false;
}

void IABaseEnnemyComponent::Receive(int message, void* data)
{

}

void IABaseEnnemyComponent::Update()
{
	stateTime += 1.0f/30.0f;
	if(currentState != previousState)
		stateTime = 0.0f;

	Automate();
}

void IABaseEnnemyComponent::Automate()
{
	// Etat WAIT
	if(currentState == S_WAIT) 
	{
		if(playerTouched)
			ChangeState(S_HIT);
		else if(stateTime > 2.0f)
			ChangeState(S_MOVE);
		else if(playerSpotted)
			ChangeState(S_ATTACK);
		else if(life <= 0)
			ChangeState(S_DEAD);
	}
	// ETAT MOVE
	else if(currentState == S_MOVE) 
	{
		if(playerTouched)
			ChangeState(S_HIT);
		else if(stateTime > 2.0f)
			ChangeState(S_WAIT);
		else if(playerSpotted)
			ChangeState(S_ATTACK);
		else if(life <= 0)
			ChangeState(S_DEAD);
	}
	// ETAT S_ATTACK
	else if(currentState == S_ATTACK) 
	{
		if(playerTouched)
			ChangeState(S_HIT);
		else if(life <= 0)
			ChangeState(S_DEAD);
	}
	// ETAT S_HIT
	else if(currentState == S_HIT) 
	{
		if(stateTime > 0.5f)
			ChangeState(S_MOVE);
		else if(life <= 0)
			ChangeState(S_DEAD);
	}
	// ETAT S_DEAD
	else if(currentState == S_DEAD) 
	{

	}
}