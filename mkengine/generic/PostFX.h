// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_POSTFX
#define MK_POSTFX

#include "lowDisplay.h"

namespace mk
{
	// Outils de manipulation de textures
	class PostFX
	{
		public:
			static void Downsample(Image* src, Image* dst, int targetw, int targeth);
			static void SimpleBlur(Image* src, Image* dst, int times);
	};
}; 

#endif