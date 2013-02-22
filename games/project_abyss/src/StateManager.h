// project_abyss
// LittleSpace Studio 2012

#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

//Includes generaux
#include <vector>
#include <unordered_map>
#include <string>

#include "gamestate.h"
#include "singleton.h"

class CGameState;
class StateManager : public Singleton<StateManager>
{
	friend class Singleton<StateManager>;

public:
    void Init();
    void cleanUp();
    void ChangeState(CGameState* state);
    void PushState(CGameState* state);
    void PopState();

    void HandleEvents();
    void Update();
    void Draw(int mode, float interpolation);

    bool Running();
    void Quit();

	void RegisterAvaiableState(std::string stname, CGameState* state);
	std::unordered_map<std::string, CGameState*>& GetAvailableStates() { return available_states; };

private:
    //La pile d'états
    std::vector<CGameState*> states;

	// Les états disponibles
	std::unordered_map<std::string, CGameState*> available_states;

    bool m_running;
};

#endif
