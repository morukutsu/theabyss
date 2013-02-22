// project_abyss
// LittleSpace Studio 2012

#include "ShipDisplayComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "ComponentsIncludes.h"
#include "../SimpleMaths.h"
#include "PlayerMovementComponent.h"

ShipDisplayComponent::ShipDisplayComponent(PlayerMovementComponent* pmov)
{
	playerMovementComponent = pmov;
	oldIsPlayerAccelerated = false;
	reactorsAngle = 0.0f;

	isVaisseauHurt = true;
	mirrorH = false;

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

	partsLookup["SHIP_PART_COCKPIT"] = SHIP_PART_COCKPIT;
	partsLookup["SHIP_PART_VIS"] = SHIP_PART_VIS;
	partsLookup["SHIP_PART_REACTEUR_FRONT"] = SHIP_PART_REACTEUR_FRONT;
	partsLookup["SHIP_PART_REACTEUR_BACK"] = SHIP_PART_REACTEUR_BACK;
	partsLookup["SHIP_FX_LUEUR"] = SHIP_FX_LUEUR;
	partsLookup["SHIP_FX_FLASHLIGHT"] = SHIP_FX_FLASHLIGHT;

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

}

void ShipDisplayComponent::Update()
{
	// Mirroring des sprites
	bool oldMirrorH = mirrorH;
	if(parent->mVel.x < -1.0f || parent->mVel.x > 1.0f)
		mirrorH = parent->mVel.x > 0;

	// Effets graphiques
	UpdateReactorsPower();
	UpdateReactorsAngle();

	Mirror(mirrorH);
	PositionSprites(mirrorH);

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
	parts_sprites[SHIP_FX_FLASHLIGHT].Show();

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

	parts_sprites[SHIP_FX_FLASHLIGHT].MoveTo((parent->mPos.x + parts_positions[SHIP_FX_FLASHLIGHT].x)/32, 
		(parent->mPos.y + parts_positions[SHIP_FX_FLASHLIGHT].y)/32);
	parts_sprites[SHIP_FX_FLASHLIGHT].Alpha(0.7f);
	parts_sprites[SHIP_FX_FLASHLIGHT].SetSize(parts[SHIP_FX_FLASHLIGHT]->getImageWidth() / 32.0f, parts[SHIP_FX_FLASHLIGHT]->getImageHeight() / 32.0f);
	parts_sprites[SHIP_FX_FLASHLIGHT].SetDepth(parent->mDepth);
	parts_sprites[SHIP_FX_FLASHLIGHT].SavePositions();
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

	parts_sprites[SHIP_FX_LUEUR].SetPriority(9 + prioShift);
	parts_sprites[SHIP_FX_LUEUR].SetBlending(MK_BLEND_ALPHA);

	// Ajout des composants générateurs de particules
	generators[GEN_REACT_A_FRONT] = new ParticleGeneratorComponent("particles/001_ship_reactor_A_front.xml");
	generators[GEN_REACT_B_FRONT] = new ParticleGeneratorComponent("particles/001_ship_reactor_B_front.xml");
	generators[GEN_REACT_A_BACK] = new ParticleGeneratorComponent("particles/001_ship_reactor_A_back.xml");
	generators[GEN_REACT_B_BACK] = new ParticleGeneratorComponent("particles/001_ship_reactor_B_back.xml");
	
	for(int k = 0; k < GEN_COUNT; k++)
	{
		parent->AddComponent(generators[k]);
		generators[k]->ignoreLightPipeline = true;
	}

	generators[GEN_REACT_A_FRONT]->priority = 3 + prioShift;
	generators[GEN_REACT_B_FRONT]->priority = 3 + prioShift;
	generators[GEN_REACT_A_BACK]->priority = 5 + prioShift;
	generators[GEN_REACT_B_BACK]->priority = 5 + prioShift;

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

	float speed = fabs(reactorsAngle - targetAngle) / 4.0f;
	if(speed >= 8.0f)
		speed = 8.0f;

	if(reactorsAngle < targetAngle)
		reactorsAngle += speed;
	else if(reactorsAngle > targetAngle)
		reactorsAngle -= speed;

	/*// Activation de l'interpolation de l'angle quand on arrete d'accélérer
	if(!playerMovementComponent->isPlayerAccelerated && oldIsPlayerAccelerated)
	{
		targetAngleInterp = 0.0f;
		sourceAngle = targetAngle;
		isAngleInterpolatingToNeutral = true;
	}

	// Et quand on accélère
	if(playerMovementComponent->isPlayerAccelerated && !oldIsPlayerAccelerated)
	{
		targetAngleInterp = 0.0f;
		sourceAngle = 0.0f;
		isAngleInterpolatingToMoving = true;
	}

	if(parent->mVel.Length() < 0.001f)
	{
		targetAngle = 0.0f;
	}

	if(isAngleInterpolatingToNeutral)
	{
		targetAngleReact = RadiansToDegrees(Slerp2D(targetAngleInterp, 0.0f, 1.0f, DegreesToRadians(sourceAngle), 0.0f));
	}
	else if(isAngleInterpolatingToMoving)
	{
		targetAngleReact = RadiansToDegrees(Slerp2D(targetAngleInterp, 0.0f, 1.0f, DegreesToRadians(sourceAngle), targetAngle));
	}
	else
	{
		if(playerMovementComponent->isPlayerAccelerated)
			targetAngleReact = targetAngle;
		else
			targetAngleReact = 0.0f;
	}

	oldIsPlayerAccelerated = playerMovementComponent->isPlayerAccelerated;
	targetAngleInterp += (1.0f/30.0f) * 4.0f;
	
	if(targetAngleInterp > 1.0f)
	{
		targetAngleInterp = 1.0f;
		isAngleInterpolatingToNeutral = false;
		isAngleInterpolatingToMoving = false;
	}*/

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

	/*static float tmprx = 0;
	static float tmpry = 0;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) )
		tmprx--;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) )
		tmprx++;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) )
		tmpry--;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) )
		tmpry++;

	printf("%f %f\n", tmprx, tmpry);*/

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

	reactorLight[0]->spr.Rotate(reactorsAngle * mx);
	reactorLight[1]->spr.Rotate(reactorsAngle * mx);

	// Rotation lampe torche
	//NVector posFlashlight = NVector(parts_positions[SHIP_FX_FLASHLIGHT].x, 
	parts_sprites[SHIP_FX_FLASHLIGHT].Rotate(reactorsAngle * mx / 180.0f * 20.0f);
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


	mk::RessourceManager::getInstance()->DeleteRessource(f);
}