// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "CutsceneEventChangeValue.h"
#include "level_manager\LevelManager.h"
#include "../Cutscene.h"
#include "CutsceneEventText.h"

CutsceneEventChangeValue::CutsceneEventChangeValue(LevelManager* lvlMan, Cutscene* cs, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters) 
	: CutsceneEvent(lvlMan, cs, eventType, time, duration)
{
	value_addr = NULL;
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
		else if((*it).first == "target_object")
		{
			target_object = (*it).second;
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
	else if(value_changed == "text_alpha")
	{
		CutsceneEvent* ev = cutscene->GetEventByName(target_object);
		if(ev != NULL)
		{
			CutsceneEventText* ev_t = dynamic_cast<CutsceneEventText*>(ev);
			value_addr = &ev_t->alpha;
		}
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

	if(value_addr != NULL)
		*value_addr = Lerp(interpTime, 0.0f, mDuration, start_value, end_value);
}

void CutsceneEventChangeValue::End() 
{

}