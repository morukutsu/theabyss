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
	mk::SpineModel* littlespace_splash;

	//Effets
	float alpha;
	float elapsedTime;

	// Musique
	mk::Music bgm;

};

#endif
