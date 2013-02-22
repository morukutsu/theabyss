// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _CHAPTERSTATE_H
#define _CHAPTERSTATE_H

#include "gamestate.h"
#include "mkengine.h"

struct Letter {
	int pos;
	float x, y, oldY;
	float alpha;
	float time;
};

class ChapterState : public CGameState
{
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents(StateManager * game);
    void Update(StateManager * game);
    void Draw(StateManager * game, int mode, float interpolation);

    static ChapterState* Instance()
    {
        return &m_ChapterState;
    }

	void InitLetters();
	void DrawLetters(float dt);
	void DrawSousTitre(float dt);
	void UpdateLetters();

	void SetChapter(std::string filename);

protected:
    ChapterState() { }

private:
    static ChapterState m_ChapterState;

	// Police titre de chapitre
	mk::Font *bigTitle, *subtitle;

	std::string chapterString, sousTitreString;
	std::vector<Letter> letters;

	float sousTitreAlpha, fadeOutAlpha;

	bool letterAnimFinished, subtitleAnimFinished;
	float subtitleTime, fadeOutTime;

	// Map à démarrer
	std::string startMap;
	int chapterId;
};

#endif
