// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef SPINE_SKELETON_H_
#define SPINE_SKELETON_H_

#include <spine/BaseSkeleton.h>
#include <SFML/Graphics/VertexArray.hpp>
#include "Image.h"
#include "Drawable.h"
#include <map>

namespace spine 
{
	class Atlas;
	class Animation;
	class Skeleton: public mk::Drawable 
	{
		public:
			Skeleton ();
			~Skeleton();

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

			Atlas* atlas_file;
			SkeletonData* skeletonData;
			BaseSkeleton* baseSkel;

			Animation* currentAnim;

			// Liste des animations
			std::map<std::string, Animation*> animations;

			float animationTime;
			int anim_mode;
	};

} /* namespace spine */
#endif /* SPINE_SKELETON_H_ */
