// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "CutsceneEventFx.h"
#include <iostream>
#include "level_manager\LevelManager.h"

CutsceneEventFx::CutsceneEventFx(LevelManager* lvlMan, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters) 
	: CutsceneEvent(lvlMan, eventType, time, duration) 
{
	for(std::map<std::string, std::string>::iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		if((*it).first == "fx_type")
		{
			fx_type = (*it).second;
		}
		else if((*it).first == "intensity")
		{
			sscanf((*it).second.c_str(), "%f", &intensity_start);
		}
		else if((*it).first == "toggle")
		{
			if((*it).second == "on")
				toggle = true;
			else
				toggle = false;
		}
	}
}

void CutsceneEventFx::Start() 
{
	if(fx_type == "blur") 
	{
		if(toggle)
		{
			levelManager->scene->blur_intensity = intensity_start;
			levelManager->scene->isEffectBlur = true;
		}
		else
		{
			levelManager->scene->isEffectBlur = false;
		}
	}
	else if(fx_type == "black_bands")
	{
		levelManager->scene->InitBlackBands();
		levelManager->scene->ActivateBlackBands(toggle);
	}
}

void CutsceneEventFx::Run(float time) 
{
}

void CutsceneEventFx::End() 
{

}