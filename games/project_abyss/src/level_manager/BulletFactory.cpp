// project_abyss
// LittleSpace Studio 2012

#include "BulletFactory.h"
#include "BulletManager.h"
#include "../SimpleMaths.h"

void BulletFactory::Init(int bullet_type, Bullet* bullet)
{
	// Fabrique de bullets
	switch(bullet_type) 
	{
		case BULLET_PULSE_LASER:
			// TODO : a optimiser
			bullet->spr.Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/projectiles/tir_avec_trainee.png") );
			bullet->light.Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/lights/light32x32.png") );
			
			int numV = 4;
			bullet->body->Initialise(NVector(bullet->originX, bullet->originY), 1.0f, bullet->vertices, numV);
			bullet->body->isSensor = true;
			bullet->body->bodytype = BODY_BULLET;

			bullet->spr.MoveTo(bullet->originX / 32.0f, bullet->originY / 32.0f);
			bullet->spr.ignoreLightPipeline = true;
			bullet->spr.SetSize(bullet->spr.image->getImageWidth() / 32.0f, bullet->spr.image->getImageHeight() / 32.0f);

			float angle = SimpleMaths::GetAngle2Points(0, 0, bullet->originVx, bullet->originVy) + 90.0f;
			bullet->spr.Rotate(angle);

			bullet->spr.Show();
			bullet->spr.Set3DMode(true);
			bullet->spr.SavePositions();

			// Light
			bullet->light.MoveTo(bullet->originX / 32.0f, bullet->originY / 32.0f);
			bullet->light.SetSize(bullet->light.image->getImageWidth() / 32.0f, bullet->light.image->getImageHeight() / 32.0f);
			bullet->light.Show();
			bullet->light.Set3DMode(true);
			bullet->light.SavePositions();

			break;
	};
}
