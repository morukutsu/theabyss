// abyss 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_CONFIGURATION_MANAGER
#define H_CONFIGURATION_MANAGER

#include "singleton.h"
#include "xml/tinyxml.h"

#include <string>
#include <map>
#include <iostream>
#include <sstream>

class ConfigurationManager : public Singleton<ConfigurationManager>
{
	friend class Singleton<ConfigurationManager>;

	public:
		bool LoadConfiguration(std::string filename);
		std::string GetAttributeString(std::string attrname);
		int GetAttributeInt(std::string attrname);

	private:
		// Tableau associatif contenant toutes les strings de configuration
		// du jeu
		std::map<std::string, std::string> mConfigurationData;

};

#endif