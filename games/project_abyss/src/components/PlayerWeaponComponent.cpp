// project_abyss
// LittleSpace Studio 2012

#include "PlayerWeaponComponent.h"
#include "PlayerInputComponent.h"
#include "../entities/Entity.h"
#include "../level_manager/BulletManager.h"
#include "../entities/EntityManager.h"
#include "../level_manager/BulletFactory.h"
#include "../level_manager/GameMap.h"
#include "../level_manager/LevelManager.h"
#include "../SimpleMaths.h"

PlayerWeaponComponent::PlayerWeaponComponent(PlayerInputComponent* _in)
{
	in = _in;
	ReadWeaponsFromXML();

	mCannonMode = C_MODE_FREE;

	reloadTime = 0.0f;
	allowShoot = true;
}

void PlayerWeaponComponent::Receive(int message, void* data)
{

}

void PlayerWeaponComponent::Update()
{
	// Tir d'une bullet
	
	// Orientation du cannon avec la souris
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	if(mCannonMode == C_MODE_FREE)
		OrientMouse(input->pointer.x, input->pointer.y);

	// Update autofire
	if(reloadTime > 0)
	{
		reloadTime -= 1.0f / 30.0f;
	}
	else
	{
		reloadTime = 0.0f;
		allowShoot = true;
	}

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
				if(mCannonMode == C_MODE_FIXED)
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
	// Autofire
	if(!allowShoot)
		return;

	reloadTime = 1.0f / wpns[0].autofire.rate;
	allowShoot = false;

	// Récupération du mirroring du héros
	bool mirrorH = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR] == 1;

	float mx = -1.0f;  
	if(mirrorH)
		mx = 1.0f;

	// Tir
	NVector velBullet = /*parent->mVel +*/ NVector(PULSE_LASER_VELOCITY*mx*cosf(mWpnAngle), PULSE_LASER_VELOCITY*sinf(mWpnAngle));

	// Calcul de la position de tir réelle relative au vaisseau
	// Arm + Cannon + shoot
	NVector armPos = NVector(wpns[0].arm_position.x, wpns[0].arm_position.y);
	NVector cannonPos = NVector(wpns[0].cannon_position.x, wpns[0].cannon_position.y);


	NVector shootPosRelative = armPos + cannonPos + NVector(wpns[0].shootX, wpns[0].shootY);
	shootPosRelative.Rotate(armPos + cannonPos, -mWpnAngle);

	// Position de tir + rotation
	NVector posBullet = NVector(shootPosRelative.x*-mx, shootPosRelative.y);

	posBullet += parent->mPos;

	BulletManager* bman = parent->GetEntityManager()->GetBulletManager();
	bman->Emit(posBullet.x, posBullet.y, velBullet.x, velBullet.y, BULLET_PULSE_LASER);
}


void PlayerWeaponComponent::Orient(int dir)
{
	switch(dir)
	{
		case CMD_ORIENT_SIDE:
			mWpnAngle = 0;
			mCannonOrientation = C_POS_NEUTRAL;
			break;
		case CMD_ORIENT_UP:
			mWpnAngle = -PI/2.0f;
			mCannonOrientation = C_POS_UP;
			break;
		case CMD_ORIENT_DOWN:
			mWpnAngle = PI/2.0f;
			mCannonOrientation = C_POS_DOWN;
			break;
		case CMD_ORIENT_UP_DIAG:
			mWpnAngle = -PI/4.0f;
			mCannonOrientation = C_POS_UP_DIAG;
			break;
		case CMD_ORIENT_DOWN_DIAG:
			mWpnAngle = PI/4.0f;
			mCannonOrientation = C_POS_DOWN_DIAG;
			break;
	};
}

void PlayerWeaponComponent::OrientMouse(float mscrx, float mscry)
{
	mCannonOrientation = C_POS_FREE;

	// Récupération du mirroring du héros
	bool mirrorH = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR] == 1;

	float mx = -1.0f;  
	if(mirrorH)
		mx = 1.0f;

	// calcul du vecteur de tir en fonction de la position du héros
	NVector heroPosition = parent->mPos;
	NVector armPos = NVector(wpns[0].arm_position.x, wpns[0].arm_position.y);
	NVector cannonPos = NVector(wpns[0].cannon_position.x, wpns[0].cannon_position.y);
	NVector shootPosRelative = armPos + cannonPos + NVector(wpns[0].shootX, wpns[0].shootY);
	shootPosRelative = NVector(shootPosRelative.x*-mx, shootPosRelative.y);
	heroPosition += shootPosRelative;

	float scrollX = parent->GetEntityManager()->GetGameMap()->GetLevelManager()->scrollX;
	float scrollY = parent->GetEntityManager()->GetGameMap()->GetLevelManager()->scrollY;

	scrollX -= mk::Core::getBaseWidth()/2.0f/32.0f;
	scrollY -= mk::Core::getBaseHeight()/2.0f/32.0f;

	NVector mouseWorldPosition = NVector(mscrx, mscry) + NVector(scrollX, scrollY)*32.0f;

	//std::cout << "Hero pos" << std::endl << "x : " << heroPosition.x << "y : " << heroPosition.y << std::endl;
	//std::cout << "Mouse World pos" << std::endl << "x : " << mouseWorldPosition.x << "y : " << mouseWorldPosition.y << std::endl;

	NVector shootVector = heroPosition - mouseWorldPosition;

	mWpnAngle = mx * SimpleMaths::GetAngle2PointsRad(shootVector.x, shootVector.y, 0, 0) + PI/2.0f;

	//std::cout << mWpnAngle << std::endl;
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

		// Arm
		tag = elem->FirstChildElement("Arm");
		std::string arm_filename = tag->Attribute("filename");
		if(arm_filename != "")
		{
			w.img_arm = arm_filename;
			double rx, ry, px, py;
			int rotation;

			tag->Attribute("rx", &rx);
			tag->Attribute("ry", &ry);
			tag->Attribute("x", &px);
			tag->Attribute("y", &py);
			tag->Attribute("rotation", &rotation);

			w.isArmRotationFixed = (rotation != 1);

			w.arm_position.x = px, w.arm_position.y = py, w.arm_position.rx = rx, w.arm_position.ry = ry;	
		}

		// Cannon
		tag = elem->FirstChildElement("Cannon");
		std::string cannon_filename = tag->Attribute("filename");
		if(cannon_filename != "")
		{
			w.img_cannon = cannon_filename;
			double rx, ry, px, py;
			int rotation;

			tag->Attribute("rx", &rx);
			tag->Attribute("ry", &ry);
			tag->Attribute("x", &px);
			tag->Attribute("y", &py);
			tag->Attribute("rotation", &rotation);

			w.isCannonRotationFixed = (rotation != 1);

			w.cannon_position.x = px, w.cannon_position.y = py, w.cannon_position.rx = rx, w.cannon_position.ry = ry;
		}

		// Shoot position
		tag = elem->FirstChildElement("ShootPosition");
		double sx, sy;
		tag->Attribute("x", &sx);
		tag->Attribute("y", &sy);

		w.shootX = sx;
		w.shootY = sy;

		// Autofire
		tag = elem->FirstChildElement("Autofire");
		tag->QueryFloatAttribute("rate", &w.autofire.rate);
		
		wpns.push_back(w);
		elem = elem->NextSiblingElement();
	}
}