// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _LEVELSTATE_H
#define _LEVELSTATE_H

#include "gamestate.h"
#include "mkengine.h"
#include "level_manager\LevelManager.h"

class LevelState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game, int mode, float interpolation);
	
	void RefreshMaps();
	void LoadMap(std::string filename);

    static LevelState* Instance()
    {
        return &m_LevelState;
    }

protected:
    LevelState() { }

private:
    static LevelState m_LevelState;

	LevelManager* lvlMan;
};

#endif
