// project_abyss
// LittleSpace Studio 2012

#ifndef MK_COMPONENT
#define MK_COMPONENT

class Entity;
class Component
{
	public:
		virtual ~Component() { };

		// Utilis� pour recevoir les messages broadcast�s depuis l'entit�
		virtual void Receive(int message, void* data) = 0;
		
		// Intialisation du composant
		virtual void Init() = 0;

		// Mise � jour du composant
		virtual void Update() = 0;

		void SetParent(Entity *p) { parent = p; };	

	protected:
		Entity*	parent;
};

#endif