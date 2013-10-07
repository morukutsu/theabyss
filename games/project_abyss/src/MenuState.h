// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "gamestate.h"
#include "mkengine.h"

class LevelManager;
class MainMenu;
class MenuState : public CGameState
{
	enum {
		STATE_PRESS_BTN, STATE_MENU
	};

	struct sParticle {
		float x, y, z;
		float vx, vy, vz;
		float alpha;
		float life;

		mk::Sprite spr;
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

	void InitParticles();
	void UpdateParticles();
	void ResetParticle(int id);

protected:
    MenuState() { }

private:
    static MenuState m_MenuState;

	// Pour la map du menu
	LevelManager* lvlMan;

	//Sprites & Images
	mk::Font* fnt, *fnt_text;

	// Effets
	float alpha;
	float elapsedTime;
	float origCamZ, camZ;
	float pressButtonTime;

	// Etat menu
	int oldState, state;
	bool changedState;
	bool firstFrame;

	// Strings du menu
	const char* mainMenuStrings[4];

	sParticle particles[64];

	// Menus
	MainMenu* mainMenu;
};

#endif
