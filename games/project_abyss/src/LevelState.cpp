// project_abyss
// LittleSpace Studio 2012

//Includes
#include "LevelState.h"
#include "StateManager.h"
#include "DebugState.h"

//GFX
LevelState LevelState::m_LevelState;

void LevelState::Init()
{
	returnToDebugState = false;
	lvlMan = new LevelManager();
}

void LevelState::Cleanup()
{
	delete lvlMan;
}

void LevelState::Pause()
{

}

void LevelState::Resume()
{

}

void LevelState::HandleEvents(StateManager* game)
{
    
}

void LevelState::Update(StateManager* game)
{
	lvlMan->Update();

	if(returnToDebugState)
	{
		lvlMan->UnloadMap();
		game->ChangeState(DebugState::Instance() );
	}
}

void LevelState::Draw(StateManager* game, int mode, float interpolation)
{	
	lvlMan->Draw(mode, interpolation);
}

void LevelState::LoadMap(std::string filename)
{
	lvlMan->Init();
	lvlMan->LoadMap(filename);
}
