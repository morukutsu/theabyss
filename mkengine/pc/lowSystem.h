// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_LOWSYSTEM
#define MK_LOWSYSTEM

#define MKE_PC				1

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "gctypes.h"
#include "Sound.h"

extern sf::Window *App;
extern sf::Clock Clock, FrameTimeClock;
extern sf::Mutex Mutex;
extern sf::Music Bgm;
extern float		mAutoDepth;
extern float mLastFrameTime;
extern bool mFullScreen;

void lowInitVideo(int w, int h, const char* name, bool fullscreen);
void lowToggleWidescreenMode(int height, bool wide);
void lowToggleVSync(bool v);
void lowSetProjectionOrtho(int w, int h);
void lowSetProjectionPerspective(int w, int h);
void lowViewport(float x, float y, float w, float h);
void lowPreUpdate();
void lowPreDisplay();
void lowRefresh();
float lowGetTime();
float lowGetFrameTime();
void lowResetFrameTimeClock();
float lowConvertFloat(float ii);
int   lowConvertInt(int val);
short lowConvertShort(short val);

void lowRunThread(void (*funcptr )(void *), void* arg);
void lowMutexLock();
void lowMutexUnlock();
void lowThreadWait();

int lowFileGets(char* dest, int size, FILE* fp);

void lowPlayOgg(const char* buffer, int len, int mode);
void lowPlaySound(mk::Sound* snd);
void lowStopOgg();

void lowError(std::string str);

void lowScreenshot(int width, int height);

#endif