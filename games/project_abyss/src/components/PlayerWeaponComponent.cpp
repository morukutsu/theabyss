// project_abyss
// LittleSpace Studio 2012

#include "PlayerWeaponComponent.h"

PlayerWeaponComponent::PlayerWeaponComponent(PlayerInputComponent* _in)
{
	in = _in;
	ReadWeaponsFromXML();
}

void PlayerWeaponComponent::Receive(int message, void* data)
{

}

void PlayerWeaponComponent::Update()
{

}

void PlayerWeaponComponent::Init()
{

}

void PlayerWeaponComponent::ReadWeaponsFromXML()
{
	mk::AsciiFile *f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/wpn_config.xml");
	TiXmlDocument doc;

	doc.Parse(f->getString() );

	TiXmlElement *root = doc.FirstChildElement("ShipWeapons");

	TiXmlElement *elem = root->FirstChildElement("Weapon");
	while(elem)
	{
		WpnInfo w;

		// Name
		TiXmlElement *tag = elem->FirstChildElement("Name");
		w.wpn_name = tag->GetText();

		// Shoot position
		tag = elem->FirstChildElement("ShootPosition");
		double sx, sy;
		elem->Attribute("x", &sx);
		elem->Attribute("y", &sy);

		w.shootX = sx;
		w.shootY = sy;

		wpns.push_back(w);

		elem = elem->NextSiblingElement();
	}
}