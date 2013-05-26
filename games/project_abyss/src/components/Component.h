// project_abyss
// LittleSpace Studio 2012

#ifndef MK_COMPONENT
#define MK_COMPONENT

class Entity;
class Component
{
	public:
		virtual ~Component() { };

		// Utilisé pour recevoir les messages broadcastés depuis l'entité
		virtual void Receive(int message, void* data) = 0;
		
		// Intialisation du composant
		virtual void Init() = 0;

		// Mise à jour du composant
		virtual void Update() = 0;

		void SetParent(Entity *p) { parent = p; };	

	protected:
		Entity*	parent;
};

#endif