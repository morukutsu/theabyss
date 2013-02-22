// project_abyss
// LittleSpace Studio 2012

#ifndef MK_STATICENTITY
#define MK_STATICENTITY

#include "Entity.h"

class StaticEntity : public Entity
{
	public:
		StaticEntity();
		~StaticEntity();

		void Init();								
		void Resolve();								
		void Cleanup();								
		void Update();								
		void Receive(int message, void* data);    
};


#endif
