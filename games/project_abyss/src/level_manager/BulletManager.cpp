// project_abyss
// LittleSpace Studio 2012

#include "BulletManager.h"

// Bullet
Bullet::Bullet()
{
	isActive = false;
	life = 0.0f;
	toDelete = false;
}

Bullet::~Bullet()
{

}

void Bullet::Init()
{

}

void Bullet::PreUpdate()
{

}

void Bullet::PostUpdate()
{

}

// BulletManager
BulletManager::BulletManager()
{
	AllocateBullets(MAX_BULLETS);
}

BulletManager::~BulletManager()
{
	FreeBullets();
}

void BulletManager::AllocateBullets(int max_bullets)
{
	bullets.reserve(max_bullets);

	firstAvailable = &bullets[0];
	for(int i = 0; i < max_bullets - 1; i++)
	{
		bullets[i].next = &bullets[i + 1];
	}
	bullets[max_bullets-1].next = NULL;
}

void BulletManager::FreeBullets()
{
	bullets.swap(bullets);
	bullets.clear();
}

int BulletManager::Emit(float x, float y, int kind)
{
	// TODO : Voir ce qu'on fait dans ce cas là ...
	assert(firstAvailable != NULL);

	Bullet* newBullet = firstAvailable;
	firstAvailable = newBullet->next;

	newBullet->Init();
}

void BulletManager::Update()
{
	// Mise à jour de chaque bullet
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(bullets[i].isActive) 
		{
			// On check s'il faut liberer la bullet
			if(bullets[i].toDelete)
				HandleBulletDelete(&bullets[i]);

			// Mise à jour
		}
	}
}

void BulletManager::HandleBulletDelete(Bullet* b)
{
	b->next = firstAvailable;
	firstAvailable = b;
}