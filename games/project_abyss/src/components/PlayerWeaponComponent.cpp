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
			case CMD_ORIENT_SIDE:
			case CMD_ORIENT_UP:
			case CMD_ORIENT_DOWN:
			case CMD_ORIENT_UP_DIAG:
			case CMD_ORIENT_DOWN_DIAG:
				Orient(command);
				break;
		}
	}	
}

void PlayerWeaponComponent::Init()
{
	
}

void PlayerWeaponComponent::Shoot()
{
	// Récupération du mirroring du héros
	bool mirrorH = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR] == 1;

	float mx = -1.0f;  
	if(mirrorH)
		mx = 1.0f;

	// Tir
	std::cout << mWpnAngle << std::endl;
	NVector velBullet = parent->mVel + NVector(PULSE_LASER_VELOCITY*mx*cosf(mWpnAngle), PULSE_LASER_VELOCITY*sinf(mWpnAngle));
	NVector posBullet = parent->mPos + NVector(wpns[0].shootX*-mx, wpns[0].shootY);

	BulletManager* bman = parent->GetEntityManager()->GetBulletManager();
	bman->Emit(posBullet.x, posBullet.y, velBullet.x, 0, BULLET_PULSE_LASER);
}


void PlayerWeaponComponent::Orient(int dir)
{
	switch(dir)
	{
		case CMD_ORIENT_SIDE:
			mWpnAngle = 0;
			//std::cout << "Orient SIDE" << std::endl;
			break;
		case CMD_ORIENT_UP:
			mWpnAngle = PI/2.0f;
			//std::cout << "Orient UP" << std::endl;
			break;
		case CMD_ORIENT_DOWN:
			mWpnAngle = (3*PI)/4.0f;
			//std::cout << "Orient DOWN" << std::endl;
			break;
		case CMD_ORIENT_UP_DIAG:
			mWpnAngle = PI/2.0f + PI/4.0f;
			//std::cout << "Orient UP DIAG" << std::endl;
			break;
		case CMD_ORIENT_DOWN_DIAG:
			mWpnAngle = PI + PI/4.0f;
			//std::cout << "Orient DOWN DIAG" << std::endl;
			break;
	};
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
		tag->Attribute("x", &sx);
		tag->Attribute("y", &sy);

		w.shootX = sx;
		w.shootY = sy;

		wpns.push_back(w);

		elem = elem->NextSiblingElement();
	}
}