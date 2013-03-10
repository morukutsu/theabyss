// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Core.h"
#include "RessourceManager.h"
#include "mkengine.h"

// Core : main engine init handling (video, sound, etc...)

namespace mk 
{
	int Core::mWindowWidth;
	int Core::mWindowHeight;
	int Core::mBaseWidth;
	int Core::mBaseHeight;
	bool Core::isRunning;
	bool Core::mWideScreenSupport;
	bool Core::isFullscreen;
	std::string Core::mWindowName;
	bool Core::isDebugVisible;
	bool Core::isLoadingFrame;

	Core::Core(int width, int height, const char* name, int renderflags)
	{
		//Init video
		isRunning = true;
		mEllapsedTimeFPS = false;
		mCurrentFPS = 0;
		mWideScreenSupport = false;
		mBaseWidth = WINDOW_BASE_WIDTH;
		mBaseHeight = WINDOW_BASE_HEIGHT;
		mPeakVramUsage = 0;

		isPointerVisible = false;
		isDebugVisible = false;

		//Déteciton des paramètres de rendu
		int viewportw, viewporth;
		if(renderflags == 0)
		{
			viewportw = width;
			viewporth = height;
		}

		if(renderflags & RENDERMODE_PAL50)
		{
			viewportw = 640;
			viewporth = 576;
		}

		if(renderflags & RENDERMODE_PAL60)
		{
			viewportw = 640;
			viewporth = 480;
		}

		if(renderflags & RENDERMODE_WIDE)
		{
			viewportw = (int)((float)viewporth*(16.0f/9.0f));
			mWideScreenSupport = true;
		}

		if(renderflags & RENDERMODE_FULLSCREEN)
		{
			isFullscreen = true;
		} 
		else
		{
			isFullscreen = false;
		}

		lowInitVideo(viewportw, viewporth, name, isFullscreen);

		mWindowWidth = viewportw;
		mWindowHeight = viewporth;
	}
	
	void Core::ToggleWidescreenMode(int h, bool wide)
	{
		if(wide)
			mWindowWidth = (int)((float)h*(16.0f/9.0f));
		else
			mWindowWidth = (int)((float)h*(4.0f/3.0f));

		mWindowHeight = h;

		lowToggleWidescreenMode(h, wide);
	}

	void Core::ToggleFullscreenMode()
	{
		isFullscreen = !isFullscreen;
		lowInitVideo(mWindowWidth, mWindowHeight, mWindowName.c_str(), isFullscreen);
	}

	Core::~Core()
	{
		
	}
	
	void Core::StartDrawing(int mode)
	{
		lowPushMatrix();

		// Calcul du scaling par rapport à l'horizontale
		ConfigureViewport();

		//Parametrage selon le mode d'affichage
		if(mode == MODE_2D)
		{
			lowSetProjectionOrtho(mWindowWidth, mBaseHeight*GetScaleFactor());
			
			// Translation (bandes noires)
			// TODO : finir gestion des écrans 4/3, 16/10
			//lowTranslate(0, -(mWindowHeight - mBaseHeight)*scaleFactor/2.0f);
			lowScale(GetScaleFactor(), GetScaleFactor());
			glDepthMask(GL_TRUE);
		}
		else if(mode == MODE_2D_BACKGROUND)
		{
			lowSetProjectionOrtho(mWindowWidth, mBaseHeight*GetScaleFactor());
			lowScale(GetScaleFactor(), GetScaleFactor());
			glDepthMask(GL_TRUE);
		}
		else
		{
			lowSetProjectionPerspective(mWindowWidth, mBaseHeight*GetScaleFactor());
			glDepthMask(GL_TRUE);
		}	
	}

	void Core::EndDrawing(int mode)
	{
		if(mode == MODE_2D) {
			// Affichage du pointeur de souris
			if(isPointerVisible) 
			{
				mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);
				mMousePointer.MoveTo(input->pointer.x, input->pointer.y);
				mMousePointer.Draw();
			}
		}

		lowPopMatrix();
	}
	
	void Core::Display()
	{
		// Refresh
		lowRefresh();

		// Screenshot
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1) )
		{
			Screenshot();
		}

		// Calcul du framerate
		mEllapsedTimeFPS += lowGetFrameTime();
		if(mEllapsedTimeFPS >= 1.0f)
		{
			mCurrentFPS = (int)((double)1.0/(double)lowGetFrameTime());
			mEllapsedTimeFPS = 0.0f;
		}
		lowResetFrameTimeClock();
	}

	void Core::Exit()
	{
		isRunning = false;
	}
	
    void Core::ConfigureViewport()
	{
		float h = (mWindowHeight - mBaseHeight*GetScaleFactor()) / 2.0f;
		lowViewport(0, h, Core::getWindowWidth(), Core::getBaseHeight()*Core::GetScaleFactor() );
	}

	void Core::ConfigureViewportFBO()
	{
		lowViewport(0, 0, Core::getWindowWidth(), Core::getWindowHeight() );
		lowSetProjectionOrtho(Core::getWindowWidth(), Core::getWindowHeight() );
	}

	void Core::Screenshot()
	{
		lowScreenshot(Core::getWindowWidth(), Core::getWindowHeight());
	}

	void Core::ShowFPS(mk::Font* fnt, float x, float y, float size, u32 color)
	{
		char txt[16];
		sprintf(txt, "%d FPS", mCurrentFPS);
		lowDisplayText(fnt, x, y, txt, color, size);
	}

	void Core::ShowEngineDebug(mk::Font* fnt)
	{
		// Affiche les informations de base de debug du moteur
		char txt[64];

		// Framerate
		sprintf(txt, "%d FPS", mCurrentFPS);
		lowDisplayText(fnt, 20, 20, txt, 0xFFFFFFFF, 0.8f);

		// Vram usage
		if(mk::RessourceManager::getInstance()->mVramUsage > mPeakVramUsage)
			mPeakVramUsage = mk::RessourceManager::getInstance()->mVramUsage;

		sprintf(txt, "VRAM : %d Mo / Peak : %d Mo", mk::RessourceManager::getInstance()->mVramUsage/1024/1024, mPeakVramUsage/1024/1024);
		lowDisplayText(fnt, 20, 40, txt, 0xFFFFFFFF, 0.8f);

		// Confidential
		lowDisplayText(fnt, (int)(mBaseWidth-300), (int)20, "project_abyss@mkengine", 0xFFFFFFFF, 0.8f);
	}

	void Core::SetPointer(mk::Image* img) 
	{
		mMousePointer.Assign(img);
	}

	void Core::TogglePointer(bool visible)
	{
		isPointerVisible = visible;
	}

	bool Core::getRunStatus()
	{
		return isRunning;
	}

	int Core::getWindowWidth()
	{
		return mWindowWidth;
	}
	
    int Core::getWindowHeight()
	{
		return mWindowHeight;
	}

	int Core::getBaseWidth()
	{
		return mBaseWidth;
	}

	int Core::getBaseHeight()
	{
		return mBaseHeight;
	}

	
};