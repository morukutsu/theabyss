// project_abyss
// LittleSpace Studio 2012

#ifndef MK_SPINE_ANIMATED_GRAPHICS_COMPONENT
#define MK_SPINE_ANIMATED_GRAPHICS_COMPONENT

#include "Component.h"
#include "mkengine.h"

class SpineAnimatedGraphicsComponent : public Component
{
	public:
		SpineAnimatedGraphicsComponent(std::string model, float scale = 1.0f, int prio = 0, std::string defaultAnim = "", float angle = 0.0f, bool mirrorX = false, bool mirrorY = false, int anim_offset = 0, bool no_shadow = false);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		mk::SpineModel model;

		float scale;
		int prio;
		float angle;
		bool mirrorX, mirrorY;
		int anim_offset;
		bool no_shadow;
		float offsetX, offsetY;
};

#endif
