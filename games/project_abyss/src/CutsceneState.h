// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _CUTSCENESTATE_H
#define _CUTSCENESTATE_H

#include "gamestate.h"
#include "mkengine.h"

class LevelManager;
class CutsceneState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game, int mode, float interpolation);

    static CutsceneState* Instance()
    {
        return &m_CutsceneState;
    }

protected:
    CutsceneState() { }

private:
    static CutsceneState m_CutsceneState;

	LevelManager* lvlMan;
};

#endif
