// abyss 
// by morukutsu (morukutsu@hotmail.com)

#include "ConfigurationManager.h"
#include "mkengine.h"

bool ConfigurationManager::LoadConfiguration(std::string filename)
{
	// Vidage du tableau
	mConfigurationData.clear();

	// Chargement des données depuis le fichier XML
	TiXmlDocument doc(filename.c_str() );
	if( !doc.LoadFile() )
	{
		lowError("gameconfig.xml not found !");
		return false;
	}

	TiXmlElement* root = doc.FirstChildElement("RootConfiguration");
	if(!root)
		return false;

	TiXmlElement* elem = root->FirstChildElement("Key");
	while(elem)
	{
		// TODO : Ajouter gestion erreurs au cas ou le fihcier est mal formé
		mConfigurationData[elem->Attribute("name")] = elem->Attribute("value");
		elem = elem->NextSiblingElement();
	}

	return true;
}

std::string ConfigurationManager::GetAttributeString(std::string attrname)
{
	return mConfigurationData[attrname];
}

int ConfigurationManager::GetAttributeInt(std::string attrname)
{
	std::istringstream iss(mConfigurationData[attrname]);
    int result;
    iss >> result;
	return result;
}