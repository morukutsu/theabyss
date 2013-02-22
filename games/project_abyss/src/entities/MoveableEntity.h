// project_abyss
// LittleSpace Studio 2012

#ifndef MK_MOVEABLEENTITY
#define MK_MOVEABLEENTITY

#include "Entity.h"

class MoveableEntity : public Entity
{
	public:
		MoveableEntity();
		~MoveableEntity();

		void Init();								
		void Resolve();								
		void Cleanup();								
		void Update();								
		void Receive(int message, void* data);    
};


#endif
