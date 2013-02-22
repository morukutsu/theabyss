// project_abyss
// LittleSpace Studio 2012

#include "ChapterState.h"
#include "RessourceManager.h"
#include "LanguageManager.h"
#include "SimpleMaths.h"
#include "xml/tinyxml.h"

#include "LevelState.h"

ChapterState ChapterState::m_ChapterState;

void ChapterState::Init()
{
	bigTitle = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/big_title.fnt");
	subtitle = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/arial_medium_bold.fnt");

	chapterString = LanguageManager::GetCString(LOC_CHAPITRE);

	sousTitreAlpha = subtitleTime = 0.0f;
	letterAnimFinished = subtitleAnimFinished = false;
	fadeOutAlpha = 0.0f;
	fadeOutTime = 0.0f;
}

void ChapterState::SetChapter(std::string filename)
{
	mk::AsciiFile* chapterFile = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);
	TiXmlDocument doc;
	doc.Parse(chapterFile->getString() );

	TiXmlElement* root = doc.FirstChildElement("Chapter");

	TiXmlElement* elem = NULL;

	// ID du chapitre
	elem = root->FirstChildElement("Id");
	sscanf(elem->GetText(), "%d", &chapterId);

	// StartMap
	elem = root->FirstChildElement("StartMap");
	startMap = elem->GetText();

	// Parametrage du chapitre manuellement ...
	switch(chapterId)
	{
		case 1:
			chapterString += " I";
			break;

		case 2:
			chapterString += " II";
			sousTitreString = LanguageManager::GetCString(LOC_CHAP_2_SOUSTITRE);
			break;
	}

	InitLetters();
}

void ChapterState::Cleanup()
{

}

void ChapterState::Pause()
{

}

void ChapterState::Resume()
{

}

void ChapterState::HandleEvents(StateManager * game)
{

}

void ChapterState::Update(StateManager * game)
{
	UpdateLetters();

	// Fin
	if(fadeOutTime > 5.0f)
	{
		fadeOutTime = 5.0f;

		// Chargement de la map suivante
		game->ChangeState(LevelState::Instance() );
		LevelState::Instance()->LoadMap(startMap );
	}
}

void ChapterState::Draw(StateManager * game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D)
	{
		DrawLetters(interpolation);
		DrawSousTitre(interpolation);

		// Fade out
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), MK_MAKE_RGBA(0, 0, 0, 255*fadeOutAlpha) );
	}
}

void ChapterState::InitLetters()
{
	std::string concat = "";
	float curX = 0.0f;

	letters.clear();
	for(int i = 0; i < chapterString.length(); i++)
	{
		Letter l;
		l.alpha = 0.0f;
		l.y = -100;
		l.pos = i;
		l.x = curX;
		l.time = 0.0f;

		concat += chapterString[i];

		curX = mk::Text::GetStringWidth(bigTitle, concat.c_str(), 1.0f);

		letters.push_back(l);
	}
}

void ChapterState::DrawLetters(float dt)
{
	float ox = 1920/2 - mk::Text::GetStringWidth(bigTitle, chapterString.c_str(), 1.0f)/2;
	float oy = 1080/2-200/2;

	for(int i = 0; i < letters.size(); i++)
	{
		std::string str = "A";
		str[0] = chapterString[i];
		float target_y = SimpleMaths::InterpolationLineaire(letters[i].oldY, letters[i].y, dt);
		lowDisplayText(bigTitle, ox + letters[i].x, oy + target_y, str.c_str(), MK_MAKE_RGBA(255, 255, 255, 255*letters[i].alpha), 1.0f);
	}
}

void ChapterState::UpdateLetters()
{
	// Animation des lettres
	bool timeUpdated = false;
	for(int i = 0; i < letters.size(); i++)
	{
		letters[i].oldY = letters[i].y;
		letters[i].y = SimpleMaths::InterpolateCosinus(-100, 0, letters[i].time);
		letters[i].alpha = SimpleMaths::InterpolateCosinus(0, 1.0f, letters[i].time);

		if(letters[i].time < 1.0f && !timeUpdated && chapterString.at(i) != ' ')
		{
			letters[i].time += 1.0f/30.0f * (i+1);
			timeUpdated = true;
		}

		if(letters[i].time > 1.0f)
			letters[i].time = 1.0f;
	}

	if(!timeUpdated)
		letterAnimFinished = true;

	// Animation du sous titre
	if(letterAnimFinished)
	{
		sousTitreAlpha = SimpleMaths::InterpolateCosinus(0, 1.0f, subtitleTime);
		subtitleTime += 1.0f/30.0f;
		if(subtitleTime > 1.0f)
		{
			subtitleTime = 1.0f;
			subtitleAnimFinished = true;
		}
	}

	// Fade out
	if(subtitleAnimFinished)
	{
		fadeOutTime += 1.0f/30.0f;
		if(fadeOutTime > 4.0f)
		{
			fadeOutAlpha = SimpleMaths::InterpolateCosinus(0, 1.0f, 4.0f-fadeOutTime);
		}
	}
}

void ChapterState::DrawSousTitre(float dt)
{
	float ox = 1920/2 - mk::Text::GetStringWidth(subtitle, sousTitreString.c_str(), 0.8f)/2;
	float oy = 1080/2 + 100;

	lowDisplayText(subtitle, ox, oy, sousTitreString.c_str(), MK_MAKE_RGBA(255, 255, 255, 255*sousTitreAlpha), 0.8f);
}