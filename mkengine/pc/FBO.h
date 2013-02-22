// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_FBO
#define MK_FBO

#include <vector>

namespace mk 
{
	class FBO 
	{
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
			std::vector<unsigned int> stencilBuffers;

			unsigned int fbo;
	};
};

#endif