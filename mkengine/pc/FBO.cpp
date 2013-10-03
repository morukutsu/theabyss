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

	void FBO::CreateFBO()
	{
		// Creation du FBO
		glGenFramebuffersEXT(1, &fbo);	
	}

	void FBO::CreateTexture(int id, int _w, int _h)
	{
		// Creation de l'indentifier FBO
		FBOtex fbotex;
		fbotex.identifier = id;
		fbotex.w = _w;
		fbotex.h = _h;

		//fbotex.texw = roundUpToNextPowerOfTwo(_w);
		//fbotex.texh = roundUpToNextPowerOfTwo(_h);

		fbotex.texw = (_w);
		fbotex.texh = (_h);

		// Creation de la texture
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &fbotex.texture);
		glBindTexture(GL_TEXTURE_2D, fbotex.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, fbotex.texw, fbotex.texh, 0, GL_RGB, GL_INT, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glDisable(GL_TEXTURE_2D);

		glGenRenderbuffersEXT(1, &fbotex.depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER, fbotex.depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_STENCIL_EXT, fbotex.texw, fbotex.texh);
		glBindRenderbufferEXT(GL_RENDERBUFFER, 0);
	
		texs.push_back(fbotex);
	}

	void FBO::Bind(int id)
	{
		glBindTexture(GL_TEXTURE_2D, texs[id].texture);
	}

	void FBO::StartDrawing(int id)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texs[id].texture, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, texs[id].depthBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, texs[id].depthBuffer);
		
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
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		for(int i = 0; i < texs.size(); i++)
		{
			glDeleteBuffers(1, &texs[i].depthBuffer);
			glDeleteTextures(1, &texs[i].texture);
		}

		glDeleteFramebuffersEXT(1, &fbo);
	}
};