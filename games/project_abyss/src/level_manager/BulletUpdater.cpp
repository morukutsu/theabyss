// project_abyss
// LittleSpace Studio 2012

#include "BulletUpdater.h"
#include "BulletManager.h"
#include "BulletFactory.h"
#include "../SimpleMaths.h"
#include "../entities/EntityManager.h"
#include "../level_manager/ParticleEnginesManager.h"
#include "../level_manager/GameMap.h"

void BulletUpdater::Update(Bullet* bullet)
{
	switch(bullet->kind) {
		case BULLET_PULSE_LASER:
			NVector mVel = bullet->body->GetDisplacement();
			NVector mPos = bullet->body->GetPosition();

			// Déplacement
			bullet->body->SetLinearVelocity(NVector(bullet->originVx, bullet->originVy));

			// Angle de l'animation
			float angle = SimpleMaths::GetAngle2Points(0, 0, bullet->originVx, bullet->originVy) + 90.0f;

			// Sprite
			bullet->spr.MoveTo(mPos.x / 32.0f, mPos.y / 32.0f);
			bullet->spr.Alpha(1.0f);
			bullet->spr.Rotate(angle);
			bullet->spr.SetSize(bullet->spr.image->getImageWidth() / 32.0f, bullet->spr.image->getImageHeight() / 32.0f);
			bullet->spr.SavePositions();
			
			// Light
			bullet->light.MoveTo(mPos.x / 32.0f, mPos.y / 32.0f);
			bullet->light.SetSize(bullet->light.image->getImageWidth() / 32.0f, bullet->light.image->getImageHeight() / 32.0f);
			bullet->light.SavePositions();

			// Collisions
			if(bullet->body->isCollision)
			{
				std::vector<CBody*>& cbodies = bullet->body->cbodies;
				for(int i = 0; i < cbodies.size(); i++)
				{
					if(cbodies[i]->bodytype == BODY_WORLD_BOUNDS || cbodies[i]->bodytype == BODY_ENNEMY)
					{
						bullet->toDelete = true;
						bullet->entMan->GetGameMap()->GetParticleManager()
							->ShowGenerator("particles/wpn_laser_expl.gen", mPos.x, mPos.y, 0.0f);
					}
				}
			}

			break;
	};
}