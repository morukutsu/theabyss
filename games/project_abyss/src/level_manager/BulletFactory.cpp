// project_abyss
// LittleSpace Studio 2012

#include "BulletFactory.h"
#include "BulletManager.h"

void BulletFactory::Init(int bullet_type, Bullet* bullet)
{
	// Fabrique de bullets
	switch(bullet_type) 
	{
		case BULLET_PULSE_LASER:
			// TODO : a optimiser
			bullet->spr.Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("sprites/vaisseau/projectiles/tir_sans_trainee.png") );
			int numV = 4;
			bullet->body->Initialise(NVector(bullet->originX, bullet->originY), 0.0f, bullet->vertices, numV);
			bullet->body->isSensor = true;
			break;
	};
}
