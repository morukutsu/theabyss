// project_abyss
// LittleSpace Studio 2012

#include "mkengine.h"
#include "StateManager.h"
#include "ConfigurationManager.h"
#include "script/ScriptManager.h"

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

	// Création des FBO
	mk::FBO::getInstance()->CreateFBO();
	
	// Full res FBOs
	mk::FBO::getInstance()->CreateTexture(0, mk::Core::getWindowWidth(), mk::Core::getWindowHeight());
	mk::FBO::getInstance()->CreateTexture(1, mk::Core::getWindowWidth(), mk::Core::getWindowHeight());

	// 1/4 res FBO
	mk::FBO::getInstance()->CreateTexture(2, mk::Core::getWindowWidth() / 4.0f, mk::Core::getWindowHeight() / 4.0f);
	mk::FBO::getInstance()->CreateTexture(3, mk::Core::getWindowWidth() / 4.0f, mk::Core::getWindowHeight() / 4.0f);

	// Acivation du fullscreen
	std::string isFullscreen = ConfigurationManager::getInstance()->GetAttributeString("Graphics_Fullscreen");
	if(isFullscreen == "true")
		mk::Core::ToggleFullscreenMode();

	// Synchro verticale
	std::string isVsync = ConfigurationManager::getInstance()->GetAttributeString("Graphics_VSync");
	if(isVsync == "true")
		mk::Core::ToggleVSync();

	//mk::RessourceManager::getInstance()->LoadTableOfContents("ressource.bin");
	std::string isCache = ConfigurationManager::getInstance()->GetAttributeString("Cache_Ressources");
	if(isCache == "false")
		mk::RessourceManager::getInstance()->SetCache(false);

	mk::InputManager::Init();

	//Statemanager initialisation
	StateManager* stman = StateManager::getInstance();
	stman->RegisterAvaiableState("SplashState", SplashState::Instance() );
	stman->RegisterAvaiableState("MenuState", MenuState::Instance() );
	stman->RegisterAvaiableState("DebugState", DebugState::Instance() );
	stman->RegisterAvaiableState("LevelState", LevelState::Instance() );
	stman->RegisterAvaiableState("ChapterState", ChapterState::Instance() );

	// Si on est en debug mode, on charge la DebugState, sinon, SplashState
	std::string isDebugMode = ConfigurationManager::getInstance()->GetAttributeString("Debug_Mode");
	if(isDebugMode == "true")
	{
		stman->ChangeState(DebugState::Instance() );
		mCore->SetDebugVisible(true);
	}
	else
		stman->ChangeState(SplashState::Instance() );

	// Affichage ou pas du DebugDraw
	std::string isDebugDraw = ConfigurationManager::getInstance()->GetAttributeString("Debug_Draw");
	if(isDebugDraw == "true") {
		mCore->SetDebugVisible(true);
	} else {
		mCore->SetDebugVisible(false);
	}

	// Activation du dev mode pour les shaders
	std::string isDebugShaders = ConfigurationManager::getInstance()->GetAttributeString("Debug_Shaders");
	if(isDebugShaders == "true") {
		mk::RessourceManager::getInstance()->SetShaderDebug(true);
	} else {
		mk::RessourceManager::getInstance()->SetShaderDebug(false);
	}

	// Chargement mapping contrôles
	mk::InputManager::setControlKeyboardMapping(0, mk::ButtonName::Up, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Up")));
	mk::InputManager::setControlKeyboardMapping(0, mk::ButtonName::Down, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Down")));
	mk::InputManager::setControlKeyboardMapping(0, mk::ButtonName::Left, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Left")));
	mk::InputManager::setControlKeyboardMapping(0, mk::ButtonName::Right, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Right")));
	mk::InputManager::setControlKeyboardMapping(0, mk::ButtonName::RT, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Shoot")));
	mk::InputManager::setEmuKey(0, mk::EMU_KEY_JOY_UP, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Orient_Up")));
	mk::InputManager::setEmuKey(0, mk::EMU_KEY_JOY_DOWN, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Orient_Down")));
	mk::InputManager::setEmuKey(0, mk::EMU_KEY_JOY_LEFT, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Orient_Left")));
	mk::InputManager::setEmuKey(0, mk::EMU_KEY_JOY_RIGHT, mk::InputManager::toKeyboardKey(ConfigurationManager::getInstance()->GetAttributeString("CtrlMap_Keyb_Orient_Right")));

	//Chagement d'une police de débug
	mk::Font* fnt;
	fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");

	// Chargement d'un pointeur de souris
	mCore->SetPointer((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/MousePointer.png") );
	mCore->TogglePointer(true);

	//Language Manager (TODO : Charger la langue sauvegardée)
	LanguageManager::LoadLanguage((mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource("lang/french.xml") );

	float nextGameUpdateTime = lowGetTime();
	int   loops = 0;

	mk::Core::SetLoadingFrame(false);
	lowResetFrameTimeClock();
	while (mCore->getRunStatus() ) 
	{
		// Update de l'état courant à un framerate fixe
		loops = 0;
		float currentTime = lowGetTime();

		lowPreUpdate();

		while (currentTime > nextGameUpdateTime && loops < FIXED_FRAMERATE)
		{
			mk::InputManager::Update();
			mCore->UpdatePointer();
			stman->HandleEvents();
			stman->Update();

			loops++;

			if(!mk::Core::GetLoadingFrame())
				nextGameUpdateTime += 1.0f/FIXED_FRAMERATE;
			else
			{
				// Pendant les frames de loading, on évite de recalculer trop de frames
				currentTime = lowGetTime();
				nextGameUpdateTime = currentTime + 1.0f/FIXED_FRAMERATE;
				mk::Core::SetLoadingFrame(false);
				break;
			}

			mk::Core::SetLoadingFrame(false);
		}

		// Calcul du facteur d'interpolation
		float interpolation = 1.0f - (nextGameUpdateTime - currentTime) / (1.0f/FIXED_FRAMERATE);

		//Péparation de l'affichage
		lowPreDisplay();
		
		// Affichage 2D (derrirèe la scene 3D)
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
		if(mk::Core::GetDebugVisible() )
			mCore->ShowEngineDebug(fnt);
		mCore->DisplayPointer(interpolation);
		mCore->EndDrawing(mk::MODE_2D);

		//draw on the screen
		mCore->Display();
	}

	// freeing ressources
	mk::RessourceManager::getInstance()->Free();

	mk::FBO::getInstance()->Destroy();

	// destroying engine
	delete mCore;
	
	return 0;
}
 
