// project_abyss
// LittleSpace Studio 2012

#include "PlayerWeaponComponent.h"
#include "PlayerInputComponent.h"
#include "../entities/Entity.h"
#include "../level_manager/BulletManager.h"
#include "../entities/EntityManager.h"
#include "../level_manager/BulletFactory.h"

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
	// Tir d'une bullet
	// Traite toutes les commandes reçues depuis l'input à cette frame
	for(std::list<int>::iterator it = in->commands.begin(); it != in->commands.end(); it++)
	{
		int command = (*it);
		switch(command)
		{
			case CMD_SHOOT:
				Shoot();
			break;
		}
	}	
}

void PlayerWeaponComponent::Init()
{

}

void PlayerWeaponComponent::Shoot()
{
	BulletManager* bman = parent->GetEntityManager()->GetBulletManager();
	bman->Emit(parent->mPos.x, parent->mPos.y, BULLET_PULSE_LASER);
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