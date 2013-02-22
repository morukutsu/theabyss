// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "FBO.h"
#include "mkengine.h"

namespace mk 
{
	FBO::FBO()
	{

	}

	FBO::~FBO()
	{
		// Destroy texture ... ...
	}

	void FBO::Create(int _w, int _h, int num)
	{
		w = _w;
		h = _h;

		texw = roundUpToNextPowerOfTwo(_w);
		texh = roundUpToNextPowerOfTwo(_h);

		textures.resize(num);
		depthBuffers.resize(num);
		stencilBuffers.resize(num);

		// Creation de la texture
		for(int i = 0; i < num; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &textures[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texw, texh, 0, GL_RGB, GL_INT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDisable(GL_TEXTURE_2D);

			// Création du Z buffer
			/*glGenRenderbuffersEXT(1, &depthBuffers[i]);
			glBindRenderbufferEXT(GL_RENDERBUFFER, depthBuffers[i]);
			glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texw, texh);
			glBindRenderbufferEXT(GL_RENDERBUFFER, 0);*/

			glGenRenderbuffersEXT(1, &depthBuffers[i]);
			glBindRenderbufferEXT(GL_RENDERBUFFER, depthBuffers[i]);
			glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_STENCIL_EXT, texw, texh);
			glBindRenderbufferEXT(GL_RENDERBUFFER, 0);

			// Creation du stencil buffer
			/*glGenRenderbuffersEXT(1, &stencilBuffers[i]);
			glBindRenderbufferEXT(GL_RENDERBUFFER, stencilBuffers[i]);
			glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_STENCIL_INDEX8_EXT, texw, texh);
			glBindRenderbufferEXT(GL_RENDERBUFFER, 0);*/
		}

		// Creation du FBO
		glGenFramebuffersEXT(1, &fbo);
	}

	void FBO::Bind(int id)
	{
		glBindTexture(GL_TEXTURE_2D, textures[id]);
	}

	void FBO::StartDrawing(int id)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textures[id], 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffers[id]);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffers[id]);
		
		/*if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "error :";
			switch(glCheckFramebufferStatus(GL_FRAMEBUFFER) )
			{
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" << std::endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
				break;
			};
		}*/

		//glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void FBO::EndDrawing()
	{
		//glPopAttrib();
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void FBO::Destroy()
	{
		glDeleteFramebuffersEXT(1, &fbo);

		for(int i = 0; i < textures.size(); i++)
		{
			glDeleteBuffers(1, &depthBuffers[i]);
			glDeleteBuffers(1, &stencilBuffers[i]);
			glDeleteTextures(1, &textures[i]);
		}
	}
};