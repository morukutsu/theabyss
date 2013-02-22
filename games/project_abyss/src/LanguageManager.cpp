// Natura 2
// by morukutsu (morukutsu@hotmail.com)

#include "LanguageManager.h"
#include "xml/tinyxml.h"

LanguageManager::LanguageManager()
{
	nullStr = "";
}

LanguageManager::~LanguageManager()
{

}

void LanguageManager::SetLanguage(int id)
{
	curLang = id;
}

void LanguageManager::LoadLanguage(mk::AsciiFile* file)
{
	//Ouverture du document
	mLanguageMap.clear();
	mLanguageMap.push_back("");

	//Parsing du fichier XML
	TiXmlDocument doc;
	doc.Parse(file->getString() );

	TiXmlElement* root = doc.FirstChildElement("Language");
	
	//Lecture données layer
	TiXmlElement* elem = root->FirstChildElement("Str");
	while(elem)
	{
		mLanguageMap.push_back(elem->GetText() );
		elem = elem->NextSiblingElement();
	}
}

std::string& LanguageManager::GetString(int txt)
{
	return mLanguageMap[txt];
}

const char* LanguageManager::GetCString(int txt)
{
	std::string& str = GetString(txt);
	return str.c_str();
}

int LanguageManager::curLang;
std::vector<std::string> LanguageManager::mLanguageMap;
std::string LanguageManager::nullStr;