// project_abyss
// LittleSpace Studio 2012

#include "mkengine.h"
#include "StateManager.h"
#include "ConfigurationManager.h"

#include "SplashState.h"
#include "MenuState.h"
#include "DebugState.h"
#include "LevelState.h"
#include "ChapterState.h"
#include "LanguageManager.h"

#define FIXED_FRAMERATE			30

//#include "LanguageManager.h"

int main(int argc, char **argv )
{
	// Loading configuration
	ConfigurationManager::getInstance()->LoadConfiguration("gameconfig.xml");

	// mkengine window initialisation
	int winW = ConfigurationManager::getInstance()->GetAttributeInt("Graphics_ScreenWidth");
	int winH = ConfigurationManager::getInstance()->GetAttributeInt("Graphics_ScreenHeight");

	mk::Core* mCore = new mk::Core(winW, winH, "project_abyss", RENDERMODE_NONE);

	//mk::RessourceManager::getInstance()->LoadTableOfContents("ressource.bin");
	//mk::InputManager::Init();

	//Statemanager initialisation
	StateManager* stman = StateManager::getInstance();
	stman->RegisterAvaiableState("SplashState", SplashState::Instance() );
	stman->RegisterAvaiableState("MenuState", MenuState::Instance() );
	stman->RegisterAvaiableState("DebugState", DebugState::Instance() );
	stman->RegisterAvaiableState("LevelState", LevelState::Instance() );
	stman->RegisterAvaiableState("ChapterState", ChapterState::Instance() );

    stman->ChangeState(DebugState::Instance() );

	//Chagement d'une police de d�bug
	mk::Font* fnt;
	fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");

	// Chargement d'un pointeur de souris
	mCore->SetPointer((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/MousePointer.png") );
	mCore->TogglePointer(true);

	//Language Manager (TODO : Charger la langue sauvegard�e)
	LanguageManager::LoadLanguage((mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("lang/french.xml") );
	
	float nextGameUpdateTime = lowGetTime();
	int   loops = 0;

	lowResetFrameTimeClock();
	while (mCore->getRunStatus() ) 
	{
		// Update de l'�tat courant � un framerate fixe
		loops = 0;
		float currentTime = lowGetTime();

		lowPreUpdate();

		while (currentTime > nextGameUpdateTime && loops < FIXED_FRAMERATE)
		{
			mk::InputManager::Update();
			stman->HandleEvents();
			stman->Update();

			loops++;
			nextGameUpdateTime += 1.0f/FIXED_FRAMERATE;
		}

		// Calcul du facteur d'interpolation
		float interpolation = 1.0f - (nextGameUpdateTime - currentTime) / (1.0f/FIXED_FRAMERATE);

		//P�paration de l'affichage
		lowPreDisplay();
		
		// Affichage 2D (derrir�e la scene 3D)
		mCore->StartDrawing(mk::MODE_2D_BACKGROUND);
		stman->Draw(mk::MODE_2D_BACKGROUND, interpolation);
		mCore->EndDrawing(mk::MODE_2D_BACKGROUND);

		// Affichage 3D
		mCore->StartDrawing(mk::MODE_3D);
		stman->Draw(mk::MODE_3D, interpolation);
		mCore->EndDrawing(mk::MODE_3D);

		// Affichage 2D (par dessus la scene 3D)
		mCore->StartDrawing(mk::MODE_2D);
		stman->Draw(mk::MODE_2D, interpolation);
		mCore->ShowEngineDebug(fnt);
		mCore->EndDrawing(mk::MODE_2D);

		//draw on the screen
		mCore->Display();
	}

	// freeing ressources
	mk::RessourceManager::getInstance()->Free();

	// destroying engine
	delete mCore;
	
	return 0;
}
 
