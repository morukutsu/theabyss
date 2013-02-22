// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Cutscene.h"
#include "cutscene_events\CutsceneEvent.h"
#include "cutscene_events\CutsceneEventFx.h"
#include "cutscene_events\CutsceneEventChangeValue.h"
#include "mkengine.h"
#include "xml/tinyxml.h"
#include <iostream>
#include "LevelManager.h"

Cutscene::Cutscene(LevelManager* lvlMan)
{
	mCurrentTime = 0;
	isFinished = false;
	levelManager = lvlMan;
}

Cutscene::~Cutscene()
{
	for(std::list<CutsceneEvent*>::iterator it = events.begin(); it != events.end(); )
	{
		delete (*it);
		events.erase(it++);
	}
}

void Cutscene::Load(std::string filename)
{
	mk::AsciiFile* cutsceneFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);

	TiXmlDocument doc;
	doc.Parse(cutsceneFile->getString() );
	
	TiXmlElement * root = doc.FirstChildElement("Cutscene");

	TiXmlElement * elem = root->FirstChildElement("Events")->FirstChildElement();
	while(elem)
	{
		double e_time_start, e_duration;
		std::string e_type;

		elem->Attribute("time", &e_time_start);
		elem->Attribute("duration", &e_duration);
		e_type = elem->Attribute("type");

		// Chargement des parametres
		TiXmlElement * parameter = elem->FirstChildElement();
		std::map<std::string, std::string> parameters;
		while(parameter)
		{
			parameters[parameter->Attribute("name")] = parameter->Attribute("value");
			parameter = parameter->NextSiblingElement();
		}

		// Fabrique
		CutsceneEventType e_event_type;
		CutsceneEvent* cutsceneEvent;
		if(e_type == "fx")
		{
			e_event_type = EVENT_FX;
			cutsceneEvent = new CutsceneEventFx(levelManager, e_event_type, e_time_start, e_duration, parameters);
		}
		else if(e_type == "change_value")
		{
			e_event_type = EVENT_CHANGE_VALUE;
			cutsceneEvent = new CutsceneEventChangeValue(levelManager, e_event_type, e_time_start, e_duration, parameters);
		}

		events.push_back(cutsceneEvent);

		elem = elem->NextSiblingElement();
	}
}

void Cutscene::Update()
{
	// Updating each effet
	for(std::list<CutsceneEvent*>::iterator it = events.begin(); it != events.end(); )
	{
		// Starting event
		if(mCurrentTime >= (*it)->mTimeStart && !(*it)->isRunning)
		{
			(*it)->Start();
			(*it)->isRunning = true;
			std::cout << "[Cutscene] t=" << mCurrentTime << " Event started" << std::endl;
		}

		if(mCurrentTime > ((*it)->mTimeStart + (*it)->mDuration) )
		{
			// Delete event
			(*it)->End();
			(*it)->isRunning = false;
			delete (*it);
			events.erase(it++);
			std::cout << "[Cutscene] t=" << mCurrentTime << " Event deleted" << std::endl;
		}
		else
		{
			// Keep runing event
			if((*it)->isRunning)
				(*it)->Run(mCurrentTime);

			it++;
		}
	}

	if(!isFinished && events.size() == 0)
	{
		std::cout << "[Cutscene] t=" << mCurrentTime << " Cutscene finished" << std::endl;
		isFinished = true;
	}

	// Incr�mentation du temps
	mCurrentTime += 1.0f/30.0f;
}