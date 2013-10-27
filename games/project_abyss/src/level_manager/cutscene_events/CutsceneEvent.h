// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CUTSCENE_EVENT
#define H_CUTSCENE_EVENT

#include <string>
#include <map>

enum CutsceneEventType 
{
	EVENT_FX, EVENT_CHANGE_VALUE, EVENT_TEXT
};

class Cutscene;
class LevelManager;
class CutsceneEvent
{
	public:
		virtual void Start() = 0;
		virtual void Run(float time) = 0;
		virtual void End() = 0;
		virtual void Draw(float interp) = 0;

		CutsceneEvent(LevelManager* lvlMan, Cutscene* cs, CutsceneEventType eventType, float time, float duration) {
			mEventType = eventType;
			mTimeStart = time;
			mDuration = duration;
			isRunning = false;
			levelManager = lvlMan;
			cutscene = cs;
		}

	public:
		bool isRunning;
		float mTimeStart, mDuration;
		CutsceneEventType mEventType;
		LevelManager* levelManager;
		std::string name;
		Cutscene* cutscene;
};

#endif