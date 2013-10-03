// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_CORE
#define MK_CORE

#include "lowSystem.h"
#include "lowDisplay.h"
#include "Sprite.h"
#include <string>

#define RENDERMODE_NONE			0x0000
#define RENDERMODE_PAL50		0x0001
#define RENDERMODE_PAL60		0x0010
#define RENDERMODE_WIDE		    0x0100
#define RENDERMODE_FULLSCREEN   0x1000

#define WINDOW_BASE_WIDTH    1920
#define WINDOW_BASE_HEIGHT   1080

// Core : main engine init handling (video, sound, etc...)
namespace mk 
{
	enum { MODE_2D, MODE_3D, MODE_2D_BACKGROUND };

	class Core
	{
		public:
			Core(int width, int height, const char* name, int rendermode);	// Engine initialization
			static void ToggleWidescreenMode(int h, bool wide);
			static void ToggleFullscreenMode();
			static void ToggleVSync();

			~Core();
			
			void StartDrawing(int mode);				// Call this before drawing something
			void EndDrawing(int mode);
			void Display();								// And call this when you want to display on the screen
			static void Exit();

			void ShowFPS(mk::Font* fnt, float x, float y, float size = 1.0f, u32 color = 0xFF0000FF);
			void ShowEngineDebug(mk::Font* fnt);
			static void SetPointer(mk::Image* img);
			static void TogglePointer(bool visible);
			static void SetDebugVisible(bool v) { isDebugVisible = v; };
			static bool GetDebugVisible() { return isDebugVisible; };

			bool getRunStatus();
			static int getWindowWidth();
			static int getWindowHeight();

			static int getBaseWidth();
			static int getBaseHeight();
			
			static float GetScaleFactor() { return (float)mWindowWidth/(float)mBaseWidth; };

			static void ConfigureViewport();
			static void ConfigureViewportFBO(FBOtex tex);
			static void Screenshot();

			static bool GetLoadingFrame() { return isLoadingFrame; };
			static void SetLoadingFrame(bool val) { isLoadingFrame = val; };

			static void UpdatePointer();
			static void DisplayPointer(float interpolation);

		private:
			static int mWindowWidth, mWindowHeight;
			static int mBaseWidth, mBaseHeight;
			static bool mWideScreenSupport;
			static bool isFullscreen;
			static bool isVSync;
			static bool isRunning;
			static std::string mWindowName;
			static bool isDebugVisible;
			static bool isLoadingFrame;

			int mCurrentFPS;
			float mEllapsedTimeFPS;
			int mPeakVramUsage;
		
			// Mouse pointer
			static mk::Sprite mMousePointer;

			static bool isPointerVisible;

		public:
			static float oldPointerX, oldPointerY, pointerX, pointerY, interpPointerX, interpPointerY;
	};
};

#endif