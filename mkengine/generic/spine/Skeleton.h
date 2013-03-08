// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef SPINE_SKELETON_H_
#define SPINE_SKELETON_H_

#include <spine/BaseSkeleton.h>
#include <SFML/Graphics/VertexArray.hpp>
#include "Image.h"
#include "Drawable.h"

namespace spine {

class Skeleton: public BaseSkeleton, public mk::Drawable {
public:
	sf::VertexArray vertexArray; // à changer
	mk::Image* texture; // BOZO - This is ugly. Support multiple textures?

	Skeleton (SkeletonData *skeletonData);

	void Draw();
	void Interpolate(float dt);
};

} /* namespace spine */
#endif /* SPINE_SKELETON_H_ */
