// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CUTSCENE_EVENT_CHANGE_VALUE
#define H_CUTSCENE_EVENT_CHANGE_VALUE

#include <string>
#include "CutsceneEvent.h"

class CutsceneEventChangeValue : public CutsceneEvent 
{
	public:
		CutsceneEventChangeValue(LevelManager* lvlMan, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters);
		void Start();
		void Run(float time);
		void End();
		void Draw(float interp) { };

	public:
		 std::string value_changed;
		 float start_value; 
		 float end_value;

	private:
		float* value_addr;
};

#endif