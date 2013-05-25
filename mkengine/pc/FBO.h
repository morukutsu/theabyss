// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_FBO
#define MK_FBO

#include <vector>
#include "singleton.h"

namespace mk 
{
	class FBO : public Singleton<FBO>
	{
		friend class Singleton<FBO>;

		public:
			FBO();
			~FBO();

			void Create(int w, int h, int num);
			void Destroy();

			void Bind(int id);
			void StartDrawing(int id);
			void EndDrawing();
			int getImageWidth() { return w; };
			int getImageHeight() { return h; };

		public:
			int w, h, texw, texh;

			// Allowing to stack multiple texture in one FBO
			std::vector<unsigned int> textures;
			std::vector<unsigned int> depthBuffers;

			unsigned int fbo;
	};
};

#endif