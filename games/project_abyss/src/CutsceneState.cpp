// project_abyss
// LittleSpace Studio 2012

#include "CutsceneState.h"
#include "level_manager\LevelManager.h"

CutsceneState CutsceneState::m_CutsceneState;

void CutsceneState::Init()
{
	lvlMan = new LevelManager();
	lvlMan->Init();

	lvlMan->isEffetTrainee = false;
	mk::Core::TogglePointer(false);

	// For debug
	lvlMan->LoadMap("maps/cutscene_00.tmx");
	lvlMan->scene->ActivateBlackBands(true);
}

void CutsceneState::Cleanup()
{

}

void CutsceneState::Pause()
{

}

void CutsceneState::Resume()
{

}

void CutsceneState::HandleEvents(StateManager * game)
{

}

void CutsceneState::Update(StateManager * game)
{
	if(lvlMan->isMapLoaded)
		lvlMan->Update();
}

void CutsceneState::Draw(StateManager * game, int mode, float interpolation)
{
	if(lvlMan->isMapLoaded)
		lvlMan->Draw(mode, interpolation);
}