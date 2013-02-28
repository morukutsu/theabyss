// project_abyss
// LittleSpace Studio 2012

//Includes
#include "SplashState.h"
#include "DebugState.h"
#include "StateManager.h"

//GFX
SplashState SplashState::m_SplashState;

void SplashState::Init()
{
	imgSplash = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/LittleSpaceStudio.png");
	sprSplash.Assign(imgSplash);
	sprRes = (mk::ExtendedSpriteRessource*)mk::RessourceManager::getInstance()->LoadRessource("littlespace_logo.spr");
	spr.Assign(sprRes);

	alpha = 0.0f;
	elapsedTime = 0.0f;
	
}

void SplashState::Cleanup()
{
	mk::RessourceManager::getInstance()->DeleteRessource(sprRes);
}

void SplashState::Pause()
{

}

void SplashState::Resume()
{

}

void SplashState::HandleEvents(StateManager* game)
{
    
}

void SplashState::Update(StateManager* game)
{
	elapsedTime += lowGetFrameTime();

	if(elapsedTime > 4)
	{
		alpha += 0.5f*lowGetFrameTime();
	}

	if(elapsedTime > 6)
		game->ChangeState(DebugState::Instance() );

	if(alpha >= 1.0f)
		alpha = 1.0f;
	if(alpha <= 0)
		alpha = 0.0f;
}

void SplashState::Draw(StateManager* game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D)
	{
		//Affichage d'un fond blanc
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 0xFFFFFFFF);
	
		spr.MoveTo(mk::Core::getBaseWidth()/2, mk::Core::getBaseHeight()/2);
		spr.Draw();
		
		// Fade
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), MK_MAKE_RGBA(255, 255, 255, 255*alpha));
	}
}