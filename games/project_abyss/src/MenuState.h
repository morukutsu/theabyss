// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "gamestate.h"
#include "mkengine.h"

class LevelManager;
class MenuState : public CGameState
{
	enum {
		STATE_PRESS_BTN, STATE_MENU
	};

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

	void UpdateCam();

protected:
    MenuState() { }

private:
    static MenuState m_MenuState;

	// Pour la map du menu
	LevelManager* lvlMan;

	//Sprites & Images
	mk::Font* fnt;

	// Effets
	float alpha;
	float elapsedTime;
	float origCamZ, camZ;

	// Etat menu
	int oldState, state;
	bool changedState;

	// Strings du menu
	const char* mainMenuStrings[4];
};

#endif
