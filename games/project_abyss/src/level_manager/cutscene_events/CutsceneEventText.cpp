// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "CutsceneEventText.h"

CutsceneEventText::CutsceneEventText(LevelManager* lvlMan, CutsceneEventType eventType, float time, float duration, std::map<std::string, std::string> parameters) 
	: CutsceneEvent(lvlMan, eventType, time, duration)
{
	alpha = 1.0f;

	for(std::map<std::string, std::string>::iterator it = parameters.begin(); it != parameters.end(); it++)
	{
		if((*it).first == "x")
		{
			sscanf((*it).second.c_str(), "%f", &x);
		}
		else if((*it).first == "y")
		{
			sscanf((*it).second.c_str(), "%f", &y);
		}
		else if((*it).first == "font")
		{
			fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource((*it).second.c_str() );
		}
		else if((*it).first == "color_r")
		{
			sscanf((*it).second.c_str(), "%d", &r);
		}
		else if((*it).first == "color_g")
		{
			sscanf((*it).second.c_str(), "%d", &g);
		}
		else if((*it).first == "color_b")
		{
			sscanf((*it).second.c_str(), "%d", &b);
		}
		else if((*it).first == "scale")
		{
			sscanf((*it).second.c_str(), "%f", &scale);
		}
		else if((*it).first == "string")
		{
			string = (*it).second;
		}
		else if((*it).first == "alpha")
		{
			sscanf((*it).second.c_str(), "%f", &alpha);
		}
	}
}

void CutsceneEventText::Start() 
{
	
}

void CutsceneEventText::Run(float time) 
{

}

void CutsceneEventText::End() 
{

}

void CutsceneEventText::Draw(float interp) 
{
	if(isRunning)
		lowDisplayText(fnt, x, y, string.c_str(), MK_MAKE_RGBA(r, g, b, 255*alpha), scale);
}