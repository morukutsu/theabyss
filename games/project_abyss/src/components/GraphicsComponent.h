// project_abyss
// LittleSpace Studio 2012

#ifndef MK_GRAPHICS_COMPONENT
#define MK_GRAPHICS_COMPONENT

#include "Component.h"
#include "mkengine.h"

class GraphicsComponent : public Component
{
	public:
		GraphicsComponent(std::string filename, float scale = 1.0f, int prio = 0, float _angle = 0.0f, bool _mirrorX = false, bool _mirrorY = false, bool no_shadow = false);

		void Receive(int message, void* data);

		void Update();

		void Init();

		void Offset(float x, float y) { offsetX = x, offsetY = y; };

	public:
		mk::Sprite spr;
		float scale;
		int prio;
		float angle;
		float offsetX, offsetY;
		bool mirrorX, mirrorY;
		bool no_shadow;
		float alpha;
};

#endif
