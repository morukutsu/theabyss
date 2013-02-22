// project_abyss
// LittleSpace Studio 2012

#ifndef MK_GRAPHICS_COMPONENT
#define MK_GRAPHICS_COMPONENT

#include "Component.h"
#include "mkengine.h"

class GraphicsComponent : public Component
{
	public:
		GraphicsComponent(std::string filename, float scale = 1.0f, int prio = 0);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		mk::Sprite spr;
		float scale;
		int prio;

};

#endif
