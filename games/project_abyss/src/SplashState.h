// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _SPLASHSTATE_H
#define _SPLASHSTATE_H

#include "gamestate.h"
#include "mkengine.h"

class SplashState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game, int mode, float interpolation);

    static SplashState* Instance()
    {
        return &m_SplashState;
    }

protected:
    SplashState() { }

private:
    static SplashState m_SplashState;

	//Sprites & Images
	mk::Image* imgSplash;
	mk::Sprite sprSplash;
	mk::ExtendedSpriteRessource* sprRes;
	mk::ExtendedSprite spr;

	spine::Skeleton *skeleton;
	spine::SkeletonData *skeletonData;
	spine::Animation *animation;

	//Effets
	float alpha;
	float elapsedTime;

};

#endif
