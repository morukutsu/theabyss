// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CUTSCENE_EVENT_FX
#define H_CUTSCENE_EVENT_FX

#include <string>
#include "CutsceneEvent.h"

class Cutscene;
class CutsceneEventFx : public CutsceneEvent 
{
	public:
		CutsceneEventFx(LevelManager* lvlMan, Cutscene* cs, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters);
		void Start();
		void Run(float time);
		void End();
		void Draw(float interp) { };

	public:
		std::string fx_type;
		float intensity_start; 
		bool toggle;
		int r, g, b;
};

#endif