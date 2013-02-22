// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "gamestate.h"
#include "mkengine.h"

class MenuState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game, int mode, float interpolation);

    static MenuState* Instance()
    {
        return &m_MenuState;
    }

protected:
    MenuState() { }

private:
    static MenuState m_MenuState;

	//Sprites & Images
	mk::Image *imgSplash, *imgBg;
	mk::Sprite sprSplash, sprBg;

	mk::ExtendedSpriteRessource* sprRes;
	mk::ExtendedSprite spr;

	// Effets
	float alpha;
	float elapsedTime;
};

#endif
