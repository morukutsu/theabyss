// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "lowSystem.h"
#include <iostream>
#include <time.h>

sf::Window *App = NULL;
sf::Clock Clock, FrameTimeClock;
sf::Mutex Mutex;
sf::Music Bgm;
float mLastFrameTime;
bool mFullScreen;

int sndRot = 0;
sf::Sound Sounds[16];

void lowInitVideo(int viewportw, int viewporth, const char* name, bool fullscreen)
{
	//Lancement
	if(App != NULL)
	{
		App->close();	
	} 
	else
	{
		lowResetFrameTimeClock();
		Clock.restart();
	}

	App = new sf::Window(sf::VideoMode(viewportw, viewporth, 32), name, sf::Style::Close | (fullscreen ? sf::Style::Fullscreen:0), sf::ContextSettings(32));
	//App->setFramerateLimit(60); // Limite à 60 images par seconde
	//App->setVerticalSyncEnabled(true);
	glewInit();

	
	App->setMouseCursorVisible(false);
	
	mFullScreen = false;

	// Initialisation des valeurs d'effacement pour les tampons de couleur et de profondeur
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	// Activation de la lecture et de l'écriture dans le tampon de profondeur
	glDepthFunc(GL_LEQUAL);							// Type Of Depth Testing
	glDisable(GL_LIGHTING);

	glViewport(0, 0, viewportw, viewporth);

	lowSetProjectionOrtho(viewportw, viewporth);

	//A changer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void lowSetProjectionOrtho(int w, int h)
{
	// Mise en place d'une projection ortho
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	

	glOrtho(0, w, h, 0, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void lowSetProjectionPerspective(int w, int h)
{
	//Projection Perspective 3D
	//glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = ((double)(w) / (double)(h));
    float verticalFieldOfViewAngle = 60.0;

    gluPerspective
    (
        verticalFieldOfViewAngle, // Field of view angle (Y angle; degrees)
        aspectRatio, // width/height
        0.01f, // distance to near clipping plane
        100.0f // distance to far clipping plane
    );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void lowToggleWidescreenMode(int h, bool wide)
{
	// Deprecated, used for old Wii support
	//Taille de la fenetre SFML
	int newsize = 0;
	if(wide)
		newsize = (int)(float)h*(16.0f/9.0f);
	else
		newsize = 640;

	App->setSize(sf::Vector2u(newsize, h) );

	//Changement OpenGL
	// Mise en place d'une projection ortho
	glMatrixMode(GL_PROJECTION);					
	glLoadIdentity();	          					
	glOrtho(0, newsize, h, 0, -1000.0f, 1000.0f);
	glViewport(0, 0, newsize, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void lowViewport(float x, float y, float w, float h)
{
	glViewport(x, y, w, h);
}

void lowPreUpdate()
{
	sf::Event Event;
	while (App->pollEvent(Event))
	{
		// Fenêtre fermée
		if (Event.type == sf::Event::Closed)
			exit(0);

	}
}

void lowPreDisplay()
{
	App->setActive();
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mAutoDepth = 0.0f;
	//glLoadIdentity();	
}

void lowRefresh()
{
	App->display();
}

float lowGetTime()
{
	return Clock.getElapsedTime().asSeconds();
}

float lowGetFrameTime()
{
	return mLastFrameTime;
}

void lowResetFrameTimeClock()
{
	mLastFrameTime = FrameTimeClock.restart().asSeconds();
}

float lowConvertFloat(float ii)
{
	return ii;
}

int lowConvertInt(int val)
{
	return val;
}

short lowConvertShort(short val)
{
	return val;
}

void lowRunThread(void(* funcptr )(void *), void* arg)
{
	/*sf::Thread* t = new sf::Thread(funcptr);
    t->launch();*/
}

void lowMutexLock()
{
	Mutex.lock();
}

void lowMutexUnlock()
{
	Mutex.unlock();
}

void lowThreadWait()
{
	//sf::sleep(1000);
}

int lowFileGets(char* dest, int size, FILE* fp)
{
	int cpt = 0;
	char curChar;

	//On lit les caractères
	while(cpt < size)
	{
		fread(&curChar, sizeof(char), 1, fp);
		if(curChar == '\n')
		{
			dest[cpt] = '\0';
			break;
		}
		else
		{
			dest[cpt] = curChar;
		}
		cpt++;
	}

	return cpt + 1;
}

void lowPlayOgg(const char* buffer, int len, int mode)
{
	Bgm.openFromMemory(buffer, len);
	Bgm.play();
	if(mode == 0)
		Bgm.setLoop(true);
}

void lowPlaySound(mk::Sound* snd) {
	Sounds[sndRot].setBuffer(snd->Buffer); // Buffer est un sfSoundBuffer
	Sounds[sndRot].play();

	sndRot++;
	sndRot = sndRot%16;
}

void lowStopOgg() {
	Bgm.stop();
}

void lowError(std::string str) {
	std::cout << "[ERROR] " << str << std::endl;

	delete App;

	getchar();

	exit(1);
}

void lowScreenshot(int width, int height)
{
	time_t t;  
    time(&t);
	char buf[64];

	std::string filename = "screen_";
	sprintf(buf, "%d", t);
	filename += buf;
	filename += ".png";

	sf::Image image;

    // copy rows one by one and flip them (OpenGL's origin is bottom while SFML's origin is top)
    std::vector<sf::Uint8> pixels(width * height * 4);
    for (int i = 0; i < height; ++i)
    {
        sf::Uint8* ptr = &pixels[i * width * 4];
        glReadPixels(0, height - i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, ptr);
    }

    image.create(width, height, &pixels[0]);

	image.saveToFile(filename);
}