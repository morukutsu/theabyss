// project_abyss
// LittleSpace Studio 2012

//Includes
#include "MenuState.h"
#include "StateManager.h"
#include "level_manager\LevelManager.h"
#include "LanguageManager.h"
#include "DebugState.h"

//GFX
MenuState MenuState::m_MenuState;

void MenuState::Init()
{
	alpha = 0.0f;
	elapsedTime = 0.0f;

	mk::Core::SetPointer((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/MousePointer.png") );
	mk::Core::TogglePointer(true);

	// Chargement de la map du menu
	lvlMan = new LevelManager();
	lvlMan->Init();
	lvlMan->LoadMap("maps/title.tmx");

	// On configure un effet
	lvlMan->scene->LoadPostFXShader("shaders/postfx_filterB_lookup.fx");
	lvlMan->scene->SetEffectIntensity(0.55f);

	camZ = 0.0f;
	origCamZ = lvlMan->gameCamera.GetDepth();

	// Police
	fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/calibri_96.fnt");

	// etat menu
	state = STATE_PRESS_BTN;

	// Strings du menu
	mainMenuStrings[0] = LanguageManager::GetCString(LOC_MENU_NEW_GAME);
	mainMenuStrings[1] = LanguageManager::GetCString(LOC_MENU_CONTINUER);
	mainMenuStrings[2] = LanguageManager::GetCString(LOC_MENU_OPTIONS);
	mainMenuStrings[3] = LanguageManager::GetCString(LOC_MENU_QUITTER);

	mk::Core::SetLoadingFrame(true);
}

void MenuState::Cleanup()
{
	delete lvlMan;
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(StateManager* game)
{
    
}

void MenuState::Update(StateManager* game)
{
	elapsedTime += 1.0f/30.0f;

	if(alpha >= 1.0f)
		alpha = 1.0f;
	if(alpha <= 0)
		alpha = 0.0f;

	// Update map
	lvlMan->Update();
	UpdateCam();

	// Gestion input
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	oldState = state;

	// Logique menus
	if(state == STATE_PRESS_BTN) 
	{
		if(input->btnPressed)
		{
			state = STATE_MENU;
			changedState = true;
		}
	} 
	else if(state == STATE_MENU) 
	{
		// Quand on arrive dans le menu général, on active les effets
		if(changedState)
		{
			lvlMan->LaunchCutscene("cutscenes/menu_press_btn.xml");
			changedState = false;
		}
	}
}

void MenuState::UpdateCam()
{
	lvlMan->gameCamera.SetDepth(origCamZ + camZ);
	camZ = sin(elapsedTime/4.0f) * 2.5f;
}

void MenuState::Draw(StateManager* game, int mode, float interpolation)
{
	// Affichage map
	lvlMan->Draw(mode, interpolation);

	if(mode == mk::MODE_2D_BACKGROUND)
	{
		//Affichage d'un fond blanc
	}
	else if(mode == mk::MODE_3D)
	{	
	}
	else if(mode == mk::MODE_2D)
	{
		// Etat menu courant
		if(state == STATE_PRESS_BTN) 
		{
			// Affichage des textes
			const char* strPressStart = LanguageManager::GetCString(LOC_MENU_APPUYEZ_TOUCHE);
			float txtPosX = mk::Core::getBaseWidth() / 2.0f - mk::Text::GetStringWidth(fnt, strPressStart, 0.75f) / 2.0f;
			float txtPosY = mk::Core::getBaseHeight() - 200;

			lowDisplayText(fnt, txtPosX, txtPosY, strPressStart, MK_MAKE_RGBA(255, 255, 255, 255), 0.75f);
		}
		else if(state == STATE_MENU)
		{
			// Affichage des menus
			for(int i = 0; i < 4; i++)
			{
				const char* s = mainMenuStrings[i];
				float txtPosX = 100.0f;
				float txtPosY = mk::Core::getBaseHeight() - 600 + 96*i;
				
				// Vert
				//lowDisplayText(fnt, txtPosX, txtPosY - 3, s, MK_MAKE_RGBA(0, 255, 0, 255), 1.0f);

				// Bleu clair
				//lowDisplayText(fnt, txtPosX - 5, txtPosY, s, MK_MAKE_RGBA(0, 255, 255, 255), 1.0f);

				// Rouge
				lowDisplayText(fnt, txtPosX -1, txtPosY -1, s, MK_MAKE_RGBA(255, 0, 0, 255), 1.05f);

				// Blanc
				lowDisplayText(fnt, txtPosX, txtPosY, s, MK_MAKE_RGBA(255, 255, 255, 255), 1.0f);
			}
		}
	}
}
