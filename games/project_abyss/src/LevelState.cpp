// project_abyss
// LittleSpace Studio 2012

//Includes
#include "LevelState.h"
#include "StateManager.h"

//GFX
LevelState LevelState::m_LevelState;

void LevelState::Init()
{

}

void LevelState::Cleanup()
{
	
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
	lvlMan.Update();
}

void LevelState::Draw(StateManager* game, int mode, float interpolation)
{	
	lvlMan.Draw(mode, interpolation);
}

void LevelState::LoadMap(std::string filename)
{
	lvlMan.Init();
	lvlMan.LoadMap(filename);
}
