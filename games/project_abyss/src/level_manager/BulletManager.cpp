// project_abyss
// LittleSpace Studio 2012

#include "BulletManager.h"
#include "BulletFactory.h"
#include "BulletUpdater.h"

#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

// Bullet
Bullet::Bullet()
{
	isActive = false;
	life = 0.0f;
	toDelete = false;
	body = new CBody();

	int numV = 4;
	vertices = PolyColl::BuildBox(numV, 32.0f, 32.0f); // Temporaire
}

Bullet::~Bullet()
{
	// body sera supprimé par GameMap
}

void Bullet::Init(int k)
{
	kind = k;
	BulletFactory::Init(kind, this);
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
	mUsedBullets = 0;
}

BulletManager::~BulletManager()
{
	FreeBullets();
}

void BulletManager::AllocateBullets()
{
	bullets.reserve(MAX_BULLETS);
	bullets.resize(MAX_BULLETS);

	firstAvailable = &bullets[0];
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		// Initialisation des paramètres de la bullet
		entityManager->GetScene()->Add(&bullets[i].spr);
		bullets[i].spr.Hide();
		
		entityManager->GetGameMap()->AddBody(bullets[i].body);
		bullets[i].body->isSleeping = true;

		// Linking
		if(i == MAX_BULLETS-1)
			bullets[i].next = NULL;
		else
			bullets[i].next = &bullets[i + 1];
	}
}

void BulletManager::FreeBullets()
{
	bullets.swap(bullets);
	bullets.clear();
}

int BulletManager::Emit(float x, float y, float vx, float vy, int kind)
{
	// TODO : Voir ce qu'on fait dans ce cas là ...
	//assert(firstAvailable != NULL);
	if(firstAvailable == NULL)
		lowError("Il n'y a plus de place pour stocker une Bullet (BulletManager.cpp, fonction 'Emit'), comme ce cas n'est pas encore gere, on quitte.");

	Bullet* newBullet = firstAvailable;
	firstAvailable = newBullet->next;

 	newBullet->originX = x, newBullet->originY = y;
	newBullet->originVx = vx, newBullet->originVy = vy;
	newBullet->kind = kind;
	newBullet->Init(kind);

	// Paramètres généraux
	newBullet->body->isSleeping = false;
	newBullet->isActive = true;

	// Ajout du body à la scène
	//entityManager->GetGameMap()->AddBody(newBullet->body);

	// Ajout du sprite


	mUsedBullets++;

	return 0;
}

void BulletManager::Update()
{
	// Mise à jour de chaque bullet
	for(int i = 0; i < MAX_BULLETS; i++)
	{
		if(bullets[i].isActive) 
		{
			// Check si la bullet est en dehors des limites du niveau
			CheckBounds(&bullets[i]);

			// On check s'il faut liberer la bullet
			if(bullets[i].toDelete)
				HandleBulletDelete(&bullets[i]);
			else
			{
				// Mise à jour
				BulletUpdater::Update(&bullets[i]);
			}
		}
	}
}

void BulletManager::CheckBounds(Bullet* b)
{
	float bx = entityManager->GetGameMap()->borderX;
	float by = entityManager->GetGameMap()->borderY;
	float bw = entityManager->GetGameMap()->borderW;
	float bh = entityManager->GetGameMap()->borderH;
	float x = b->body->GetPosition().x;
	float y = b->body->GetPosition().y;

	if(x < bx || y < by || x > bx + bw || y > by + bh)
		b->toDelete = true;
}

void BulletManager::HandleBulletDelete(Bullet* b)
{
	b->isActive = false;
	b->toDelete = false;
	b->spr.Hide();
	b->body->isSleeping = true;
	b->next = firstAvailable;
	firstAvailable = b;
	mUsedBullets--;
}