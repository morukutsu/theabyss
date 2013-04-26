// project_abyss
// LittleSpace Studio 2012

#include "mkengine.h"
#include <spine/spine.h>

#ifndef SPINE_MODEL_H_
#define SPINE_MODEL_H_

namespace mk
{
	class SpineModel: public mk::Drawable 
	{
		public:
			SpineModel ();
			~SpineModel();

			void LoadModelDescriptorFile(std::string filename);
			void PlayAnim(std::string anim, int mode);

			void Draw();
			void Play();

			void MoveTo(float x, float y) { posX = x, posY = y; };
			void Scale(float w, float h)  { scaleX = w, scaleY = h; };
			void Rotate(float Angle) { angle = Angle; };
			void SetDepth(float z) { posZ = z; };

			void Interpolate(float dt);
			void SavePositions();

		public:
			sf::VertexArray vertexArray;
			mk::Image* texture;

			spine::Atlas* atlas_file;
			spine::Skeleton* skeleton_file;
			spine::SkeletonData* skeletonData;

			spine::AnimationState* state;

			spine::Animation* currentAnim;

			// Liste des animations
			//std::map<std::string, spine::Animation*> animations;

			float animationTime, oldAnimationTime;
			int anim_mode;
	};
};

#endif