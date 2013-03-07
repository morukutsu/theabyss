// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ANIMATED_GRAPHICS_COMPONENT
#define MK_ANIMATED_GRAPHICS_COMPONENT

#include "Component.h"
#include "mkengine.h"

class AnimatedGraphicsComponent : public Component
{
	public:
		AnimatedGraphicsComponent(std::string model, float scale, int prio, std::string defaultAnim, float angle, bool mirrorX, bool mirrorY, int anim_offset, bool no_shadow);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		mk::ModelBone model;
		mk::MeshBone* mesh;
		mk::MeshBoneAnim* anim;
		mk::Image* tex;

		float scale;
		int prio;
		float angle;
		bool mirrorX, mirrorY;
		int anim_offset;
		bool no_shadow;
};

#endif
