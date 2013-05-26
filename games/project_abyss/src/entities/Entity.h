// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENTITY
#define MK_ENTITY

#include "../physics/Vector.h"
#include <list>
#include <string>
#include <vector>

class Component;
class mk::Scene;
class EntityManager;
class CBody;
class Entity
{
	public:
		Entity() { mId = -1; toDelete = false; isActive = true; mDepth = 0.0f; sensorBody = NULL; lifetime = -1.0f; };
		virtual ~Entity();

		virtual void Init() = 0;								// Loading
		virtual void Resolve() = 0;								// Initialisation des positions etc
		virtual void Cleanup() = 0;								// Unloading
		virtual void Update() = 0;								// Mise � jour
		virtual void Receive(int message, void* data) = 0;      // Utile pour recevoir les messages broadcast�s depuis le manager
		
		void Delete() { toDelete = true; };						// Pr�paration � la suppression de l'objet
		void Send(int message, void* data);
		void SendToManager(int message, void* data);
		void AddComponent(Component *comp);
		void SetId(int id) { mId = id; };
		void SetScene(mk::Scene* scene) { parentScene = scene; };
		mk::Scene* GetScene() { return parentScene; };
		void SetEntityManager(EntityManager * m) { entityManager = m; };
		EntityManager* GetEntityManager() { return entityManager; };
		void AddWaypoint(Entity* e) { waypoints.push_back(e); };

		void InitComponents();
		void UpdateComponents();

		void SetSensorBody(CBody* s) { sensorBody = s; };

		NVector mPos;											// Position de l'entit� dans le monde
		NVector mVel;											// Vitesse de l'entit�
		float mWidth, mHeight;									// Largeur hauteur de l'entit� (lu depuis tiled)
		float mDepth;											// Profondeur de l'entit�

		// Nom et type
		std::string mName, mType;

		// Collisions
		CBody* sensorBody;

		// Waypoints
		std::vector<Entity*> waypoints;

		// Temps de vie de l'entit�
		float lifetime;
		bool toDelete;

	protected:
		int mId;												// Identifiant de l'instance de l'entit�
		
		// Composants
		std::list<Component*> components;						// Composants de l'entit�

		// pointeur sur la scene parente
		mk::Scene* parentScene;

		// pointeur sur le manager d'entit�s parent
		EntityManager* entityManager;

		// Triggers
		bool isActive;	
};

#endif