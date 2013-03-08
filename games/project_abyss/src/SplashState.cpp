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
	//sprRes = (mk::ExtendedSpriteRessource*)mk::RessourceManager::getInstance()->LoadRessource("gui/littlespace_logo.spr");
	//spr.Assign(sprRes);

	alpha = 0.0f;
	elapsedTime = 0.0f;

	/*spine::Skeleton *skeleton;
	spine::SkeletonData *skeletonData;
	spine::Animation *animation;*/
	
	mk::AsciiFile* atlasFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("test/spineboy.atlas");
	std::string tmp = atlasFile->getString();
	tmp = tmp.substr(0, atlasFile->getSize());

	spine::Atlas *atlas = new spine::Atlas(tmp );

	spine::SkeletonJson skeletonJson(atlas);

	mk::AsciiFile* skeletonFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("test/spineboy-skeleton.json");
	tmp = skeletonFile->getString();
	tmp = tmp.substr(0, skeletonFile->getSize());

	skeletonData = skeletonJson.readSkeletonData(tmp );

	mk::AsciiFile* animationFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("test/spineboy-walk.json");
	tmp = animationFile->getString();
	tmp = tmp.substr(0, animationFile->getSize());

	animation = skeletonJson.readAnimation(tmp, skeletonData);

	skeleton = new spine::Skeleton(skeletonData);
	skeleton->flipX = false;
	skeleton->flipY = false;
	skeleton->setToBindPose();
	skeleton->getRootBone()->x = 200;
	skeleton->getRootBone()->y = 420;
	skeleton->updateWorldTransform();
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

	/*if(elapsedTime > 6)
		game->ChangeState(DebugState::Instance() );*/

	if(alpha >= 1.0f)
		alpha = 1.0f;
	if(alpha <= 0)
		alpha = 0.0f;

	animation->apply(skeleton, 0.0f, true);
	skeleton->updateWorldTransform();
}

void SplashState::Draw(StateManager* game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D)
	{
		//Affichage d'un fond blanc
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 0xFFFFFFFF);
	
		/*spr.MoveTo(mk::Core::getBaseWidth()/2, mk::Core::getBaseHeight()/2);
		spr.Draw();*/

		skeleton->Draw();
		
		// Fade
		//lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), MK_MAKE_RGBA(255, 255, 255, 255*alpha));
	}
}
