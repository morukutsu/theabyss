// project_abyss
// LittleSpace Studio 2012

#include "BulletUpdater.h"
#include "BulletManager.h"
#include "BulletFactory.h"

void BulletUpdater::Update(Bullet* bullet)
{
	switch(bullet->kind) {
		case BULLET_PULSE_LASER:
			NVector mVel = bullet->body->GetDisplacement();
			NVector mPos = bullet->body->GetPosition();

			bullet->body->SetLinearVelocity(NVector(bullet->originVx, 0));

			bullet->spr.MoveTo(mPos.x / 32.0f, mPos.y / 32.0f);
			bullet->spr.Alpha(1.0f);
			bullet->spr.Rotate(0.0f);
			bullet->spr.SetSize(bullet->spr.image->getImageWidth() / 32.0f, bullet->spr.image->getImageHeight() / 32.0f);
			bullet->spr.SavePositions();
			

			break;
	};
}