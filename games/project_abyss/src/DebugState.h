// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _DEBUGSTATE_H
#define _DEBUGSTATE_H

#include "gamestate.h"
#include "mkengine.h"

class DebugMenu;
class DebugState : public CGameState
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

    static DebugState* Instance()
    {
        return &m_DebugState;
    }

protected:
    DebugState() { }

private:
    static DebugState m_DebugState;

	DebugMenu *debugMenu, *statesSubMenu, *mapsSubMenu, *chaptersSubMenu;
	bool isFirstLoad;

};

#endif
