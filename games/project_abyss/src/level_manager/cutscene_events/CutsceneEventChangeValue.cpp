// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "CutsceneEventChangeValue.h"
#include "level_manager\LevelManager.h"

CutsceneEventChangeValue::CutsceneEventChangeValue(LevelManager* lvlMan, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters) 
	: CutsceneEvent(lvlMan,eventType, time, duration)
{
	for(std::map<std::string, std::string>::iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		if((*it).first == "value_changed")
		{
			value_changed = (*it).second;
		}
		else if((*it).first == "start_value")
		{
			sscanf((*it).second.c_str(), "%f", &start_value);
		}
		else if((*it).first == "end_value")
		{
			sscanf((*it).second.c_str(), "%f", &end_value);
		}
	}
}

void CutsceneEventChangeValue::Start() 
{
	if(value_changed == "blur_intensity")
	{
		value_addr = &levelManager->scene->blur_intensity;
	}
	else if(value_changed == "fx_intensity")
	{
		value_addr = &levelManager->scene->effectIntensity;
	}
	else if(value_changed == "plane_opacity")
	{
		value_addr = &levelManager->scene->planeOpacity;
	}
	else
	{
		std::cout << "[ERR. Cutscene] parametre non connu" << value_changed << std::endl;
		lowError("Quit");
	}
}

void CutsceneEventChangeValue::Run(float time) 
{
	float interpTime = time - mTimeStart;

	*value_addr = Lerp(interpTime, 0.0f, mDuration, start_value, end_value);
}

void CutsceneEventChangeValue::End() 
{

}