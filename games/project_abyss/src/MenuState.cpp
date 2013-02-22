// project_abyss
// LittleSpace Studio 2012

//Includes
#include "MenuState.h"
#include "StateManager.h"

//GFX
MenuState MenuState::m_MenuState;

void MenuState::Init()
{
	imgSplash = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/title_screen_logo.png");
	sprSplash.Assign(imgSplash);

	imgBg = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/title_screen_bg.png");
	sprBg.Assign(imgBg);

	//sprRes = (mk::ExtendedSpriteRessource*)mk::RessourceManager::getInstance()->LoadRessource("littlespace_logo.spr");
	//spr.Assign(sprRes);

	alpha = 0.0f;
	elapsedTime = 0.0f;
	
	//scene.Add(&sprSplash);
}

void MenuState::Cleanup()
{
	mk::RessourceManager::getInstance()->DeleteRessource(imgSplash);
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(StateManager* game)
{
    
}

void MenuState::Update(StateManager* game)
{
	elapsedTime += lowGetFrameTime();

	if(alpha >= 1.0f)
		alpha = 1.0f;
	if(alpha <= 0)
		alpha = 0.0f;
}

void MenuState::Draw(StateManager* game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D_BACKGROUND)
	{
		//Affichage d'un fond blanc
		//lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 0xFFFFFFFF);
		sprBg.MoveTo(0, 0);
		sprBg.Draw();
	}
	else if(mode == mk::MODE_3D)
	{
		mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);	
	}
	else if(mode == mk::MODE_2D)
	{

	}
}
