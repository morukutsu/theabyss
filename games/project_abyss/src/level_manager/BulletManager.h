// project_abyss
// LittleSpace Studio 2012

#ifndef MK_BULLET_MANAGER
#define MK_BULLET_MANAGER

#include "../physics/Vector.h"
#include "../physics/Body.h"

#include <vector> 
#include "mkengine.h"

#define MAX_BULLETS			128

class Bullet
{
	public:
		Bullet();
		~Bullet();

	public:
		NVector pos;
		CBody* body;		// Adresse vers un Body déjà alloué
		bool isActive;
};

class BulletManager 
{
	public:
		// Initialisation
		BulletManager();
		~BulletManager();
		void AllocateBullets(int max_bullets);
		void FreeBullets();

		// Création / Destruction de bullets
		int Emit(float x, float y, int kind);	// Retourne l'ID de la bullet créee, sinon -1

		// Mise à jour des bullets (appel des visiteurs sur chaque bullet)
		void Update();

	public:
		// Tableau de bullets
		std::vector<Bullet> bullets;
};

#endif