// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_FBO
#define MK_FBO

#include <vector>
#include "singleton.h"

namespace mk 
{
	struct FBOtex
	{
		// Identifiant utile du FBO
		int identifier;

		// Identifiants OpenGL
		unsigned int texture, depthBuffer;
		
		// Largeur et hauteur du FBO
		int w, h;
		int texw, texh;
	};

	class FBO : public Singleton<FBO>
	{
		friend class Singleton<FBO>;

		public:
			FBO();
			~FBO();

			void CreateFBO();
			void CreateTexture(int identifier, int w, int h);
			void Destroy();

			void Bind(int id);
			void StartDrawing(int id);
			void EndDrawing();

		public:
			// Allowing to stack multiple texture in one FBO
			std::vector<FBOtex> texs;
			unsigned int fbo;
	};
};

#endif