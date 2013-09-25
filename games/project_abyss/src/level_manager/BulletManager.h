// project_abyss
// LittleSpace Studio 2012

#ifndef MK_BULLET_MANAGER
#define MK_BULLET_MANAGER

#include "../physics/Vector.h"
#include "../physics/Body.h"

#include <vector> 
#include "mkengine.h"

#define MAX_BULLETS			64

class EntityManager;

class Bullet
{
	public:
		Bullet();
		~Bullet();

		void Init(int bullet_kind);		// Initialise la bullet

		void PreUpdate();
		void PostUpdate();

	public:
		int kind;
		bool isActive, toDelete;
		float life;			// Temps de vie de la particule

		// Quand la ubllet est utilisée
		float originX, originY, originVx, originVy;
		CBody* body;
		NVector* vertices;
		mk::Sprite spr, light;

		EntityManager* entMan;

		// Quand la bullet est inactive
		Bullet *next;	
};


class BulletManager 
{
	public:
		// Initialisation
		BulletManager();
		~BulletManager();
		void SetEntityManager(EntityManager* mgr) { entityManager = mgr; };

		void AllocateBullets();
		void FreeBullets();

		// Création / Destruction de bullets
		int Emit(float x, float y, float vx, float vy, int kind);	// Retourne l'ID de la bullet créee, sinon -1

		// Mise à jour des bullets (appel des visiteurs sur chaque bullet)
		void Update();

	private:
		void HandleBulletDelete(Bullet* b);
		void CheckBounds(Bullet* b);

	public:
		// Tableau de bullets
		std::vector<Bullet> bullets;

		// Accès au bullets libres
		Bullet* firstAvailable;

		// Ptr vers l'entity manager
		EntityManager* entityManager;

		// Used bullets
		int mUsedBullets;
};

#endif