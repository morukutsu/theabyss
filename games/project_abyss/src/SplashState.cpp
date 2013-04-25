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
	alpha = 0.0f;
	elapsedTime = 0.0f;

	littlespace_splash = new mk::SpineModel();
	littlespace_splash->LoadModelDescriptorFile("test/littlespace_logo.xml");
	littlespace_splash->PlayAnim("animation", ANIM_ONCE);

	littlespace_splash->posX = mk::Core::getBaseWidth()/2.0f;
	littlespace_splash->posY = mk::Core::getBaseHeight()/2.0f;

	mk::Core::SetLoadingFrame(true);
}

void SplashState::Cleanup()
{
	delete littlespace_splash;
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
	elapsedTime += 1.0f/30.0f;

	if(elapsedTime > 4)
	{
		alpha += 0.1f;
	}

	if(elapsedTime > 6)
	{
		game->ChangeState(DebugState::Instance() );
		return;
	}

	if(alpha >= 1.0f)
		alpha = 1.0f;
	if(alpha <= 0)
		alpha = 0.0f;

	littlespace_splash->Play();
}

void SplashState::Draw(StateManager* game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D)
	{
		//Affichage d'un fond blanc
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 0xFFFFFFFF);
	
		littlespace_splash->Interpolate(interpolation);
		littlespace_splash->Draw();
		
		// Fade
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), MK_MAKE_RGBA(255, 255, 255, 255*alpha));
	}
}
