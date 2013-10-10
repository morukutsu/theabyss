// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CUTSCENE
#define H_CUTSCENE

#include <string>
#include <map>
#include <list>

class LevelManager;
class CutsceneEvent;
class Cutscene 
{
	public:
		Cutscene(LevelManager* lvlMan);
		~Cutscene();

		void Load(std::string filename);
		void Update();
		void Draw(float interp);

	public:
		float mCurrentTime;
		std::list<CutsceneEvent*> events;
		bool isFinished;
		LevelManager* levelManager;

};

#endif H_CUTSCENE