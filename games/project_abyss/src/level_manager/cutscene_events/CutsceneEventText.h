// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CUTSCENE_EVENT_TEXT
#define H_CUTSCENE_EVENT_TEXT

#include <string>
#include "CutsceneEvent.h"
#include "mkengine.h"

class Cutscene;
class CutsceneEventText : public CutsceneEvent 
{
	public:
		CutsceneEventText(LevelManager* lvlMan, Cutscene* cs, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters);
		void Start();
		void Run(float time);
		void End();
		void Draw(float interp);

	public:
		int r, g, b;
		float x, y;
		mk::Font *fnt;
		std::string string;
		float scale;
		float alpha;

		mk::TextDynamicLetters* dynamicText;

	private:

};

#endif