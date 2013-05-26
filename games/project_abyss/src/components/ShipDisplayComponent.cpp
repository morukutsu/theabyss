// project_abyss
// LittleSpace Studio 2012

#include "ShipDisplayComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "ComponentsIncludes.h"
#include "../SimpleMaths.h"
#include "PlayerMovementComponent.h"
#include "PlayerWeaponComponent.h"
#include "../entities/EntityManager.h"

ShipDisplayComponent::ShipDisplayComponent(PlayerMovementComponent* pmov, PlayerWeaponComponent* pwpn)
{
	playerMovementComponent = pmov;
	playerWeaponComponent = pwpn;

	oldIsPlayerAccelerated = false;
	reactorsAngle = 0.0f;

	isVaisseauHurt = true;
	
	// Chargement des images
	parts[SHIP_PART_COCKPIT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/vaisseau.png");
	parts[SHIP_PART_COCKPIT_HURT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/hurt/vaisseau.png");
	parts[SHIP_PART_VIS] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/vis.png");
	parts[SHIP_PART_VIS_HURT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/hurt/vis.png");
	parts[SHIP_PART_REACTEUR_FRONT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/reacteur_front.png");
	parts[SHIP_PART_REACTEUR_FRONT_HURT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/hurt/reacteur_front.png");
	parts[SHIP_PART_REACTEUR_BACK] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/reacteur_back.png");
	parts[SHIP_PART_REACTEUR_BACK_HURT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/hurt/reacteur_back.png");
	parts[SHIP_FX_LUEUR] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/lueur.png");
	parts[SHIP_FX_FLASHLIGHT] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/torchlight.png");
	parts[SHIP_PART_TORCHE] = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/armes/lamp.png");

	partsLookup["SHIP_PART_COCKPIT"] = SHIP_PART_COCKPIT;
	partsLookup["SHIP_PART_VIS"] = SHIP_PART_VIS;
	partsLookup["SHIP_PART_REACTEUR_FRONT"] = SHIP_PART_REACTEUR_FRONT;
	partsLookup["SHIP_PART_REACTEUR_BACK"] = SHIP_PART_REACTEUR_BACK;
	partsLookup["SHIP_FX_LUEUR"] = SHIP_FX_LUEUR;
	partsLookup["SHIP_FX_FLASHLIGHT"] = SHIP_FX_FLASHLIGHT;
	partsLookup["SHIP_PART_TORCHE"] = SHIP_PART_TORCHE;

	partsLookup["REACTOR_LIGHT_0"] = SHIP_PARTS_COUNT + REACTOR_LIGHT_0;
	partsLookup["REACTOR_LIGHT_1"] = SHIP_PARTS_COUNT + REACTOR_LIGHT_1;

	partsLookup["FLASHLIGHT_MASK"] = SHIP_PARTS_COUNT + FLASHLIGHT_MASK;
	partsLookup["BACKLIGHT_MASK"] = SHIP_PARTS_COUNT + BACKLIGHT_MASK;

	// On lit les positions depuis le XML
	ReadPositionsFromXML();
}

ShipDisplayComponent::~ShipDisplayComponent()
{
	partsPositionsXML.swap(partsPositionsXML);

	for(int i = 0; i < wpns.size(); i++)
	{
		delete wpns[i].spr_arm;
		delete wpns[i].spr_cannon;
	}
}

void ShipDisplayComponent::PositionSprites(bool v)
{
	float mx = 1.0f;
	if(v)
		mx = -mx;

	// Centres de rotation des sprites
	parts_centers[SHIP_PART_COCKPIT] = NVector(partsPositionsXML[SHIP_PART_COCKPIT].rx * mx, partsPositionsXML[SHIP_PART_COCKPIT].ry);
	parts_centers[SHIP_PART_COCKPIT_HURT] = parts_centers[SHIP_PART_COCKPIT];

	parts_centers[SHIP_PART_VIS] = NVector(partsPositionsXML[SHIP_PART_VIS].rx * mx, partsPositionsXML[SHIP_PART_VIS].ry);
	parts_centers[SHIP_PART_VIS_HURT] = parts_centers[SHIP_PART_VIS];

	parts_centers[SHIP_PART_REACTEUR_FRONT] = NVector(partsPositionsXML[SHIP_PART_REACTEUR_FRONT].rx * mx, partsPositionsXML[SHIP_PART_REACTEUR_FRONT].ry);
	parts_centers[SHIP_PART_REACTEUR_FRONT_HURT] = parts_centers[SHIP_PART_REACTEUR_FRONT];

	parts_centers[SHIP_PART_REACTEUR_BACK] = NVector(partsPositionsXML[SHIP_PART_REACTEUR_BACK].rx * mx, partsPositionsXML[SHIP_PART_REACTEUR_BACK].ry);
	parts_centers[SHIP_PART_REACTEUR_BACK_HURT] = parts_centers[SHIP_PART_REACTEUR_BACK];

	parts_centers[SHIP_FX_LUEUR] = NVector(partsPositionsXML[SHIP_FX_LUEUR].rx, partsPositionsXML[SHIP_FX_LUEUR].ry);

	parts_centers[SHIP_FX_FLASHLIGHT] = NVector(partsPositionsXML[SHIP_FX_FLASHLIGHT].rx * mx, partsPositionsXML[SHIP_FX_FLASHLIGHT].ry);

	parts_centers[SHIP_PART_TORCHE] = NVector(partsPositionsXML[SHIP_PART_TORCHE].rx * mx, partsPositionsXML[SHIP_PART_TORCHE].ry);

	// Positions des parties du vaisseau par rapport au centre
	parts_positions[SHIP_PART_COCKPIT] = NVector(partsPositionsXML[SHIP_PART_COCKPIT].x, partsPositionsXML[SHIP_PART_COCKPIT].y);
	parts_positions[SHIP_PART_COCKPIT_HURT] = parts_positions[SHIP_PART_COCKPIT];

	parts_positions[SHIP_PART_VIS] = NVector(partsPositionsXML[SHIP_PART_VIS].x * mx, partsPositionsXML[SHIP_PART_VIS].y);
	parts_positions[SHIP_PART_VIS_HURT] = parts_positions[SHIP_PART_VIS];

	parts_positions[SHIP_PART_REACTEUR_FRONT] = NVector(partsPositionsXML[SHIP_PART_REACTEUR_FRONT].x  * mx, partsPositionsXML[SHIP_PART_REACTEUR_FRONT].y);
	parts_positions[SHIP_PART_REACTEUR_FRONT_HURT] = parts_positions[SHIP_PART_REACTEUR_FRONT];

	parts_positions[SHIP_PART_REACTEUR_BACK] = NVector(partsPositionsXML[SHIP_PART_REACTEUR_BACK].x * mx, partsPositionsXML[SHIP_PART_REACTEUR_BACK].y);
	parts_positions[SHIP_PART_REACTEUR_BACK_HURT] = parts_positions[SHIP_PART_REACTEUR_BACK];

	parts_positions[SHIP_FX_LUEUR] = NVector(partsPositionsXML[SHIP_FX_LUEUR].x, partsPositionsXML[SHIP_FX_LUEUR].y);

	parts_positions[SHIP_FX_FLASHLIGHT] = NVector(partsPositionsXML[SHIP_FX_FLASHLIGHT].x * -mx, partsPositionsXML[SHIP_FX_FLASHLIGHT].y);

	parts_positions[SHIP_PART_TORCHE] = NVector(partsPositionsXML[SHIP_PART_TORCHE].x * -mx, partsPositionsXML[SHIP_PART_TORCHE].y);

	generators[GEN_REACT_A_FRONT]->offsetX = 66 * mx/32.0f + parent->mPos.x/32.0f;
	generators[GEN_REACT_A_FRONT]->offsetY = 38/32.0f + parent->mPos.y/32.0f;
	generators[GEN_REACT_A_FRONT]->offsetZ = parent->mDepth;

	generators[GEN_REACT_B_FRONT]->offsetX = 66 * mx/32.0f + parent->mPos.x/32.0f;
	generators[GEN_REACT_B_FRONT]->offsetY = 38/32.0f + parent->mPos.y/32.0f;
	generators[GEN_REACT_B_FRONT]->offsetZ = parent->mDepth;

	generators[GEN_REACT_A_BACK]->offsetX = 62 * mx/32.0f + parent->mPos.x/32.0f;
	generators[GEN_REACT_A_BACK]->offsetY = 25/32.0f + parent->mPos.y/32.0f;
	generators[GEN_REACT_A_BACK]->offsetZ = parent->mDepth;

	generators[GEN_REACT_B_BACK]->offsetX = 62 * mx/32.0f + parent->mPos.x/32.0f;
	generators[GEN_REACT_B_BACK]->offsetY = 25/32.0f + parent->mPos.y/32.0f;
	generators[GEN_REACT_B_BACK]->offsetZ = parent->mDepth;

	for(int k = 0; k < SHIP_PARTS_COUNT; k++)
	{
		parts_sprites[k].SetRotCenter(parts_centers[k].x / 32.0f, parts_centers[k].y / 32.0f);
		parts_shadow[k].SetRotCenter(parts_centers[k].x / 32.0f, parts_centers[k].y / 32.0f);
	}

}

void ShipDisplayComponent::Receive(int message, void* data)
{
	if(message == MSG_HERO_KILL)
	{
		// On désactive les générateurs de particules
		generators[GEN_REACT_A_BACK]->isActive = false;
		generators[GEN_REACT_B_BACK]->isActive = false;
		generators[GEN_REACT_A_FRONT]->isActive = false;
		generators[GEN_REACT_B_FRONT]->isActive = false;
		generators[GEN_SMOKE]->isActive = true;
		parts_sprites[SHIP_FX_FLASHLIGHT].Hide();
		parts_sprites[SHIP_PART_TORCHE].Hide();
		parent->GetEntityManager()->AddParticleImage(parent->mPos.x, parent->mPos.y, "particles/explosion.png", 0.15f);
		parent->GetScene()->ShakeScreen(10.0f, 0.5f);
		//flashLight->spr.Hide();
		reactorLight[0]->spr.Hide();
		reactorLight[1]->spr.Hide();
	}
}

void ShipDisplayComponent::Update()
{
	// Mirroring des sprites
	mirrorH = playerMovementComponent->mirrorH;
	bool oldMirrorH = playerMovementComponent->oldMirrorH;

	// Effets graphiques
	UpdateReactorsPower();
	
	Mirror(mirrorH);
	PositionSprites(mirrorH);

	UpdateReactorsAngle();

	// Affichage des différentes parties
	for(int i = 0; i < 2; i++)
	{
		bool shadow = i == 1;
		getSprite(SHIP_PART_COCKPIT, shadow).Show();
		getSprite(SHIP_PART_VIS, shadow).Show();
		getSprite(SHIP_PART_REACTEUR_FRONT, shadow).Show();
		getSprite(SHIP_PART_REACTEUR_BACK, shadow).Show();
	}
	parts_sprites[SHIP_FX_LUEUR].Show();

	for(int k = 0; k < SHIP_MAIN_PARTS_COUNT; k+=2)
	{
		for(int i = 0; i < 2; i++)
		{
			bool shadow = i == 1;
			getSprite(k, shadow).MoveTo((parent->mPos.x + parts_positions[k].x)/32, (parent->mPos.y + parts_positions[k].y)/32);
			getSprite(k, shadow).Alpha(1.0f);
			getSprite(k, shadow).SetSize(parts[k]->getImageWidth() / 32.0f, parts[k]->getImageHeight() / 32.0f);
			getSprite(k, shadow).SetDepth(parent->mDepth);
			getSprite(k, shadow).SavePositions();

			// Si un mirror a été effectué pendant cette frame on ne l'interpole pas
			if(oldMirrorH != mirrorH)
				getSprite(k, shadow).SavePositions();
		}
	}

	parts_sprites[SHIP_FX_LUEUR].MoveTo((parent->mPos.x + parts_positions[SHIP_FX_LUEUR].x)/32, 
		(parent->mPos.y + parts_positions[SHIP_FX_LUEUR].y)/32);
	parts_sprites[SHIP_FX_LUEUR].SetSize(parts[SHIP_FX_LUEUR]->getImageWidth() / 32.0f, parts[SHIP_FX_LUEUR]->getImageHeight() / 32.0f);
	parts_sprites[SHIP_FX_LUEUR].SetDepth(parent->mDepth);
	parts_sprites[SHIP_FX_LUEUR].Alpha(1.0f);
	parts_sprites[SHIP_FX_LUEUR].SavePositions();

	parts_sprites[SHIP_FX_FLASHLIGHT].Alpha(0.7f);
	parts_sprites[SHIP_FX_FLASHLIGHT].SetSize(parts[SHIP_FX_FLASHLIGHT]->getImageWidth() / 32.0f, parts[SHIP_FX_FLASHLIGHT]->getImageHeight() / 32.0f);
	parts_sprites[SHIP_FX_FLASHLIGHT].SetDepth(parent->mDepth);
	parts_sprites[SHIP_FX_FLASHLIGHT].SavePositions();

	// Image lampe torche
	parts_sprites[SHIP_PART_TORCHE].SetSize(parts[SHIP_PART_TORCHE]->getImageWidth() / 32.0f, parts[SHIP_PART_TORCHE]->getImageHeight() / 32.0f);
	parts_sprites[SHIP_PART_TORCHE].SetDepth(parent->mDepth);
	parts_sprites[SHIP_PART_TORCHE].Alpha(1.0f);
	parts_sprites[SHIP_PART_TORCHE].SavePositions();

	// Update de l'arme courante
	UpdateWeapon();
}

void ShipDisplayComponent::UpdateWeapon()
{
	// Gesiton du mirroring
	float mx = 1.0f;
	if(mirrorH)
		mx = -mx;

	currentWeapon = 0;
	wpns[currentWeapon].spr_arm->Show();
	wpns[currentWeapon].spr_cannon->Show();

	wpns[currentWeapon].spr_arm->MoveTo((parent->mPos.x + wpns[currentWeapon].arm_position.x * mx) / 32.0f, (parent->mPos.y + wpns[currentWeapon].arm_position.y) / 32.0f);
	wpns[currentWeapon].spr_arm->Alpha(1.0f);
	wpns[currentWeapon].spr_arm->SetDepth(parent->mDepth);
	wpns[currentWeapon].spr_arm->SetSize(wpns[currentWeapon].spr_arm->image->getImageWidth() / 32.0f, wpns[currentWeapon].spr_arm->image->getImageHeight() / 32.0f); 
	wpns[currentWeapon].spr_arm->Mirror(!mirrorH, false);

	if(!wpns[currentWeapon].isArmRotationFixed)
		wpns[currentWeapon].spr_arm->Rotate(0.0f);

	wpns[currentWeapon].spr_arm->SavePositions();
	
	// Rotation du cannon
	NVector cannon_position = NVector(wpns[currentWeapon].cannon_position.x * mx / 32.0f + wpns[currentWeapon].arm_position.x * mx / 32.0f, 
		wpns[currentWeapon].cannon_position.y / 32.0f + wpns[currentWeapon].arm_position.y / 32.0f);

	if(!wpns[currentWeapon].isCannonRotationFixed)
	{
		cannon_position.Rotate(parts_positions[SHIP_PART_VIS], 0.0f);

		// On récupére l'orientation depuis le playerwpn
		float cannonAngle = -playerWeaponComponent->mWpnAngle + PI;

		wpns[currentWeapon].spr_cannon->Rotate(RadiansToDegrees(cannonAngle)*mx );
	}

	cannon_position += parent->mPos / 32.0f;

	wpns[currentWeapon].spr_cannon->MoveTo(cannon_position.x, cannon_position.y);
	wpns[currentWeapon].spr_cannon->Alpha(1.0f);
	wpns[currentWeapon].spr_cannon->SetDepth(parent->mDepth);
	wpns[currentWeapon].spr_cannon->SetSize(wpns[currentWeapon].spr_cannon->image->getImageWidth() / 32.0f, wpns[currentWeapon].spr_cannon->image->getImageHeight() / 32.0f); 
	wpns[currentWeapon].spr_cannon->Mirror(mirrorH, false);
	wpns[currentWeapon].spr_cannon->SavePositions();
}

void ShipDisplayComponent::UpdateShadowSprites()
{

}


void ShipDisplayComponent::Init()
{
	// Création des sprites
	for(int k = 0; k < SHIP_PARTS_COUNT; k++)
	{
		parts_sprites[k].Assign(parts[k]);
		parts_sprites[k].SetSize(parts[k]->getImageWidth() / 32.0f, parts[k]->getImageHeight() / 32.0f);
		parts_sprites[k].SetRotCenter(parts_centers[k].x / 32.0f, parts_centers[k].y / 32.0f);
		parent->GetScene()->Add(&parts_sprites[k]);
		parts_sprites[k].Set3DMode(true);
		parts_sprites[k].SetDepth(parent->mDepth);
		parts_sprites[k].Hide();
	}

	// Création des sprites shadow
	for(int k = 0; k < SHIP_MAIN_PARTS_COUNT; k++)
	{
		parts_shadow[k].Assign(parts[k]);
		parts_shadow[k].SetSize(parts[k]->getImageWidth() / 32.0f, parts[k]->getImageHeight() / 32.0f);
		parts_shadow[k].SetRotCenter(parts_centers[k].x / 32.0f, parts_centers[k].y / 32.0f);
		parent->GetScene()->AddLight(&parts_shadow[k]);
		parts_shadow[k].Set3DMode(true);
		parts_shadow[k].SetDepth(parent->mDepth);
		parts_shadow[k].Hide();
		parts_shadow[k].SetPriority(-2);
		parts_shadow[k].Tint(0, 0, 0);
		parts_shadow[k].SetBlending(MK_BLEND_ALPHA);
		parts_shadow[k].SetStencilClipped(true);
	}

	// Positionnement en profondeur
	int prioShift = 0; // -12 over 
	parts_sprites[SHIP_PART_REACTEUR_BACK].SetPriority(4 + prioShift);
	parts_sprites[SHIP_PART_COCKPIT].SetPriority(3 + prioShift);
	parts_sprites[SHIP_PART_REACTEUR_FRONT].SetPriority(2 + prioShift);
	parts_sprites[SHIP_PART_VIS].SetPriority(1 + prioShift);

	parts_sprites[SHIP_PART_REACTEUR_BACK_HURT].SetPriority(4 + prioShift);
	parts_sprites[SHIP_PART_COCKPIT_HURT].SetPriority(3 + prioShift);
	parts_sprites[SHIP_PART_REACTEUR_FRONT_HURT].SetPriority(2 + prioShift);
	parts_sprites[SHIP_PART_VIS_HURT].SetPriority(1 + prioShift);

	parts_sprites[SHIP_PART_TORCHE].SetPriority(4 + prioShift);

	parts_sprites[SHIP_FX_LUEUR].SetPriority(9 + prioShift);
	parts_sprites[SHIP_FX_LUEUR].SetBlending(MK_BLEND_ALPHA);

	// Ajout des composants générateurs de particules
	generators[GEN_REACT_A_FRONT] = new ParticleGeneratorComponent("particles/001_ship_reactor_A_front.xml");
	generators[GEN_REACT_B_FRONT] = new ParticleGeneratorComponent("particles/001_ship_reactor_B_front.xml");
	generators[GEN_REACT_A_BACK] = new ParticleGeneratorComponent("particles/001_ship_reactor_A_back.xml");
	generators[GEN_REACT_B_BACK] = new ParticleGeneratorComponent("particles/001_ship_reactor_B_back.xml");
	generators[GEN_SMOKE] = new ParticleGeneratorComponent("particles/smoke.xml");
	generators[GEN_SMOKE]->isActive = false;

	for(int k = 0; k < GEN_COUNT; k++)
	{
		parent->AddComponent(generators[k]);
		generators[k]->ignoreLightPipeline = true;
	}

	generators[GEN_REACT_A_FRONT]->priority = 3 + prioShift;
	generators[GEN_REACT_B_FRONT]->priority = 3 + prioShift;
	generators[GEN_REACT_A_BACK]->priority = 5 + prioShift;
	generators[GEN_REACT_B_BACK]->priority = 5 + prioShift;
	generators[GEN_SMOKE]->priority = -3 + prioShift;

	reactorVelOrig[0] = NVector(generators[GEN_REACT_A_FRONT]->vx, generators[GEN_REACT_A_FRONT]->vy);
	reactorVelOrig[1] = NVector(generators[GEN_REACT_B_FRONT]->vx, generators[GEN_REACT_B_FRONT]->vy);

	// Flashlight
	flashLight = new LightComponent("sprites/vaisseau/light_torch.png");
	flashLight->spr.SetPriority(-3 + prioShift);
	parent->AddComponent(flashLight);
	
	// Backlight
	backLight = new LightComponent("sprites/vaisseau/light_lueur.png");
	backLight->spr.SetPriority(-1 + prioShift); // en dessous, 10
	parent->AddComponent(backLight);

	// Les deux lights des reacteurs
	reactorLight[0] = new LightComponent("sprites/vaisseau/reactor_light.png");
	reactorLight[1] = new LightComponent("sprites/vaisseau/reactor_light.png");
	
	reactorLight[0]->spr.SetPriority(-3 + prioShift); // 6
	reactorLight[1]->spr.SetPriority(6 + prioShift); // 10

	parent->AddComponent(reactorLight[0]);
	parent->AddComponent(reactorLight[1]);

	parts_sprites[SHIP_FX_FLASHLIGHT].SetPriority(-3 + prioShift);
	parts_sprites[SHIP_FX_FLASHLIGHT].ignoreLightPipeline = true;

	ReadWeaponsFromXML();

	mirrorH = parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR] == 1;

	// Affichage de quelques sprites
	parts_sprites[SHIP_FX_FLASHLIGHT].Show();
	parts_sprites[SHIP_PART_TORCHE].Show();
}

void ShipDisplayComponent::Mirror(bool v)
{
	// Gesiton du mirroring
	float mx = 1.0f;
	if(mirrorH)
		mx = -mx;

	for(int k = 0; k < SHIP_MAIN_PARTS_COUNT; k+=2)
	{
		getSprite(k, false).Mirror(v, false);
		getSprite(k, true).Mirror(v, false);
	}

	for(int k = 0; k < GEN_COUNT; k++)
	{
		generators[k]->MirrorLocal(v, false);
	}

	parts_sprites[SHIP_FX_LUEUR].Mirror(!v, false);

	flashLight->Mirror(!v, false);
	flashLight->Offset(partsPositionsXML[partsLookup["FLASHLIGHT_MASK"]].x*-mx, partsPositionsXML[partsLookup["FLASHLIGHT_MASK"]].y);

	backLight->Mirror(!v, false);
	backLight->Offset(partsPositionsXML[partsLookup["BACKLIGHT_MASK"]].x*-mx, partsPositionsXML[partsLookup["BACKLIGHT_MASK"]].y);

	reactorLight[0]->Mirror(v, false);
	reactorLight[1]->Mirror(v, false);

	parts_sprites[SHIP_FX_FLASHLIGHT].Mirror(!v, false);

	parts_sprites[SHIP_PART_TORCHE].Mirror(!v, false);
}

void ShipDisplayComponent::UpdateReactorsAngle()
{
	// Gesiton du mirroring
	float mx = 1.0f;
	if(mirrorH)
		mx = -mx;

	// Selon vel Y on change la position des réacteurs
	float targetAngle = SimpleMaths::GetAngle2Points(0, 0, -fabs(parent->mVel.x), parent->mVel.y) - 90.0f;

	if(parent->mVel.Length() < 0.001f || !playerMovementComponent->isPlayerAccelerated)
	{
		targetAngle = 0.0f;
	}

	// En cas de knockback
	if(playerMovementComponent->isKnockback)
	{
		targetAngle = targetAngle - 90;
	}

	float speed = fabs(reactorsAngle - targetAngle) / 4.0f;
	if(speed >= 8.0f)
		speed = 8.0f;

	if(reactorsAngle < targetAngle)
		reactorsAngle += speed;
	else if(reactorsAngle > targetAngle)
		reactorsAngle -= speed;

	for(int i = 0; i < 2; i++)
	{
		bool shadow = i == 1;

		getSprite(SHIP_PART_COCKPIT, shadow).Rotate(reactorsAngle * mx / 180.0f * 20.0f);
		getSprite(SHIP_PART_VIS, shadow).Rotate(reactorsAngle * mx / 180.0f * 20.0f);

		getSprite(SHIP_PART_REACTEUR_FRONT, shadow).Rotate(reactorsAngle * mx);
		getSprite(SHIP_PART_REACTEUR_BACK, shadow).Rotate(reactorsAngle * mx);
	}

	generators[GEN_REACT_A_FRONT]->RotateLocal(-48/32.0f, -6/32.0f, reactorsAngle);
	generators[GEN_REACT_A_BACK]->RotateLocal(-48/32.0f, -6/32.0f, reactorsAngle);
	generators[GEN_REACT_B_FRONT]->RotateLocal(-48/32.0f, -6/32.0f, reactorsAngle);
	generators[GEN_REACT_B_BACK]->RotateLocal(-48/32.0f, -6/32.0f, reactorsAngle);

	NVector posLightReactorFront = NVector(partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_0].x * mx, 
										   partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_0].y);

	posLightReactorFront.Rotate(NVector((partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_0].rx) * mx, 
										(partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_0].ry)), 
										DegreesToRadians(reactorsAngle * mx));

	NVector posLightReactorBack = NVector(partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_1].x * mx, 
		partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_1].y);

	posLightReactorBack.Rotate(NVector((partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_1].rx) * mx, 
		(partsPositionsXML[SHIP_PARTS_COUNT + REACTOR_LIGHT_1].ry)), 
		DegreesToRadians(reactorsAngle * mx));

	reactorLight[0]->Offset(posLightReactorFront.x, posLightReactorFront.y);
	reactorLight[1]->Offset(posLightReactorBack.x, posLightReactorBack.y);

	reactorLight[0]->spr.SetRotCenter(-28/32.0f * mx, 0.0f);
	reactorLight[1]->spr.SetRotCenter(-28/32.0f * mx, 0.0f);

	reactorLight[0]->SetAngle(reactorsAngle * mx);
	reactorLight[1]->SetAngle(reactorsAngle * mx);

	// Rotation lampe torche (Lumière)
	NVector posFlashlight = NVector((parts_positions[SHIP_PART_TORCHE].x)/32.0f, (parts_positions[SHIP_PART_TORCHE].y)/32.0f);

	posFlashlight.Rotate(NVector(27/32.0f * mx, 34/32.0f), DegreesToRadians(reactorsAngle * mx / 180.0f * 20.0f) );
	posFlashlight += parent->mPos/32.0f + NVector((parts_positions[SHIP_FX_FLASHLIGHT].x)/32, (parts_positions[SHIP_FX_FLASHLIGHT].y)/32);

	parts_sprites[SHIP_FX_FLASHLIGHT].MoveTo(posFlashlight.x, posFlashlight.y);
	parts_sprites[SHIP_FX_FLASHLIGHT].Rotate(reactorsAngle * mx / 180.0f * 20.0f);

	// Rotation lampe torche (lampe)
	NVector posLamp = NVector((parts_positions[SHIP_PART_TORCHE].x)/32, (parts_positions[SHIP_PART_TORCHE].y)/32);
	posLamp.Rotate(NVector(27/32.0f * mx, 34/32.0f), DegreesToRadians(reactorsAngle * mx / 180.0f * 20.0f));
	posLamp += parent->mPos/32.0f;

	parts_sprites[SHIP_PART_TORCHE].MoveTo(posLamp.x, posLamp.y);
	parts_sprites[SHIP_PART_TORCHE].Rotate(reactorsAngle * mx / 180.0f * 20.0f);

	// Pour la lightpass
	flashLight->spr.SetRotCenter((partsPositionsXML[SHIP_PARTS_COUNT + FLASHLIGHT_MASK].rx * mx)/32.0f, partsPositionsXML[SHIP_PARTS_COUNT + FLASHLIGHT_MASK].ry/32.0f);
	flashLight->SetAngle(reactorsAngle * mx / 180.0f * 20.0f);
}

void ShipDisplayComponent::UpdateReactorsPower()
{
	// Gesiton du mirroring
	float mx = 1.0f;
	if(mirrorH)
		mx = -mx;

	// Ajout de puissance aux réacteurs
	NVector normalisedShipVelocity = parent->mVel;
	normalisedShipVelocity.x = normalisedShipVelocity.x / 15.0f;
	normalisedShipVelocity.y = normalisedShipVelocity.y / 15.0f;

	reactorVel[0].x = reactorVelOrig[0].x + normalisedShipVelocity.Length() * 3.0f; 
	reactorVel[0].y = reactorVelOrig[0].y; 

	reactorVel[1].x = reactorVelOrig[1].x + normalisedShipVelocity.Length() * 3.0f;
	reactorVel[1].y = reactorVelOrig[1].y; 

	generators[GEN_REACT_A_FRONT]->vx = reactorVel[0].x;
	generators[GEN_REACT_A_FRONT]->vy = reactorVel[0].y;

	generators[GEN_REACT_A_BACK]->vx = reactorVel[0].x;
	generators[GEN_REACT_A_BACK]->vy = reactorVel[0].y;

	generators[GEN_REACT_B_FRONT]->vx = reactorVel[1].x;
	generators[GEN_REACT_B_FRONT]->vy = reactorVel[1].y;

	generators[GEN_REACT_B_BACK]->vx = reactorVel[1].x;
	generators[GEN_REACT_B_BACK]->vy = reactorVel[1].y;
}

mk::Sprite& ShipDisplayComponent::getSprite(int constant, bool shadow)
{
	int diff = 0;
	if(isVaisseauHurt)
		diff = 1;

	mk::Sprite* tableau;
	if(shadow)
		tableau = parts_shadow;
	else
		tableau = parts_sprites;

	switch(constant)
	{
		case SHIP_PART_COCKPIT:
			return tableau[SHIP_PART_COCKPIT + diff];
			break;

		case SHIP_PART_VIS:
			return tableau[SHIP_PART_VIS + diff];
			break;

		case SHIP_PART_REACTEUR_FRONT:
			return tableau[SHIP_PART_REACTEUR_FRONT + diff];
			break;

		case SHIP_PART_REACTEUR_BACK:
			return tableau[SHIP_PART_REACTEUR_BACK + diff];
			break;
	}
}

void ShipDisplayComponent::ReadPositionsFromXML()
{
	partsPositionsXML.resize(32);

	mk::AsciiFile *f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/ship_config.xml");
	TiXmlDocument doc;

	doc.Parse(f->getString() );

	TiXmlElement *root = doc.FirstChildElement("Ship");

	TiXmlElement *elem = root->FirstChildElement("Element");
	while(elem)
	{
		PartPosition part;

		std::string e_name = elem->Attribute("name");

		double x, y, rx, ry;

		elem->Attribute("x", &x);
		elem->Attribute("y", &y);
		elem->Attribute("rx", &rx);
		elem->Attribute("ry", &ry);

		part.x = (float)x;
		part.y = (float)y;
		part.rx = (float)rx;
		part.ry = (float)ry;

		partsPositionsXML[partsLookup[e_name]] = part;

		elem = elem->NextSiblingElement();
	}
}

void ShipDisplayComponent::ReadWeaponsFromXML()
{
	mk::AsciiFile *f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/wpn_config.xml");
	TiXmlDocument doc;

	doc.Parse(f->getString() );

	TiXmlElement *root = doc.FirstChildElement("ShipWeapons");

	TiXmlElement *elem = root->FirstChildElement("Weapon");
	while(elem)
	{
		WpnDisplay w;

		// Name
		TiXmlElement *tag = elem->FirstChildElement("Name");
		w.wpn_name = tag->GetText();

		// Arm
		tag = elem->FirstChildElement("Arm");
		std::string arm_filename = tag->Attribute("filename");
		if(arm_filename != "")
		{
			mk::Image* img = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(arm_filename);
			double rx, ry, px, py;
			int rotation;

			tag->Attribute("rx", &rx);
			tag->Attribute("ry", &ry);
			tag->Attribute("x", &px);
			tag->Attribute("y", &py);
			tag->Attribute("rotation", &rotation);

			w.isArmRotationFixed = (rotation != 1);

			w.arm_position.x = px, w.arm_position.y = py, w.arm_position.rx = rx, w.arm_position.ry = ry;

			w.spr_arm = new mk::Sprite;
			w.spr_arm->Assign(img);
			w.spr_arm->SetSize(img->getImageWidth() / 32.0f, img->getImageHeight() / 32.0f);
			w.spr_arm->SetRotCenter(w.arm_position.rx/ 32.0f, w.arm_position.ry/ 32.0f);

		}

		// Cannon
		tag = elem->FirstChildElement("Cannon");
		std::string cannon_filename = tag->Attribute("filename");
		if(cannon_filename != "")
		{
			mk::Image* img = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(cannon_filename);
			double rx, ry, px, py;
			int rotation;

			tag->Attribute("rx", &rx);
			tag->Attribute("ry", &ry);
			tag->Attribute("x", &px);
			tag->Attribute("y", &py);
			tag->Attribute("rotation", &rotation);

			w.isCannonRotationFixed = (rotation != 1);

			w.cannon_position.x = px, w.cannon_position.y = py, w.cannon_position.rx = rx, w.cannon_position.ry = ry;

			w.spr_cannon = new mk::Sprite;
			w.spr_cannon->Assign(img);
			w.spr_cannon->SetSize(img->getImageWidth() / 32.0f, img->getImageHeight() / 32.0f);
			w.spr_cannon->SetRotCenter(w.cannon_position.rx/ 32.0f, w.cannon_position.ry/ 32.0f);
		}
		
		w.spr_cannon->Set3DMode(true);
		w.spr_arm->Set3DMode(true);

		w.spr_arm->SetPriority(2); // + prioShift ...
		w.spr_cannon->SetPriority(3);

		wpns.push_back(w);

		// Ajout des sprites à la scène
		parent->GetScene()->Add(wpns[wpns.size()-1].spr_arm);
		parent->GetScene()->Add(wpns[wpns.size()-1].spr_cannon);
		wpns[wpns.size()-1].spr_cannon->Hide();
		wpns[wpns.size()-1].spr_arm->Hide();

		elem = elem->NextSiblingElement();
	}
}