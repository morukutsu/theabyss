// project_abyss
// LittleSpace Studio 2012

//Includes
#include "DebugState.h"
#include "StateManager.h"
#include "debug_tools\DebugMenu.h"
#include "debug_tools\FileLister.h"

#include "LevelState.h"
#include "ChapterState.h"

DebugState DebugState::m_DebugState;



void DebugState::Init()
{
	mk::Font* fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");
	
	// Main menu
	debugMenu = new DebugMenu(fnt);
	debugMenu->AddItem(new DebugItemButton("Changer de GameState"));
	debugMenu->AddItem(new DebugItemButton("Charger une map"));
	debugMenu->AddItem(new DebugItemButton("Démarrer un chapitre"));
	debugMenu->AddItem(new DebugItemButton("Quitter le DebugMenu"));
	debugMenu->AddItem(new DebugItemButton("Quitter le jeu"));

	// States sub menu
	statesSubMenu = new DebugMenu(fnt);
	statesSubMenu->Hide();
	statesSubMenu->ToggleFocus(false);

	// Maps sub menu
	mapsSubMenu = new DebugMenu(fnt);
	mapsSubMenu->Hide();
	mapsSubMenu->ToggleFocus(false);

	// Chapitres
	chaptersSubMenu = new DebugMenu(fnt);
	chaptersSubMenu->AddItem(new DebugItemButton("Chapitre 2"));
	chaptersSubMenu->AddItem(new DebugItemButton("Retour"));
	chaptersSubMenu->Hide();
	chaptersSubMenu->ToggleFocus(false);

	isFirstLoad = true;

	
	mk::Core::SetPointer((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/MousePointer.png") );
	mk::Core::TogglePointer(true);

	bgm.Open("sound/music/01_the_big_sea_ambiance.ogg");
	bgm.Play();
}

void DebugState::Cleanup()
{
	
}

void DebugState::Pause()
{

}

void DebugState::Resume()
{

}

void DebugState::HandleEvents(StateManager* game)
{
    
}

void DebugState::Update(StateManager* game)
{
	if(isFirstLoad) 
	{
		std::unordered_map<std::string, CGameState*>& statesList = game->GetAvailableStates();
		for(std::unordered_map<std::string, CGameState*>::iterator it = statesList.begin(); it != statesList.end(); it++)
		{
			statesSubMenu->AddItem(new DebugItemButton((*it).first));
		}
		statesSubMenu->AddItem(new DebugItemButton("Retour"));
	}
	isFirstLoad = false;

	// Selection d'un item du menu principal
	int id = debugMenu->GetPressedItemId();
	if(id != -1)
	{
		if(id == 0) 
		{
			debugMenu->ToggleFocus(false);
			statesSubMenu->ToggleFocus(true);
			statesSubMenu->Show();
		}
		else if(id == 1)
		{
			debugMenu->ToggleFocus(false);
			RefreshMaps();
			mapsSubMenu->ToggleFocus(true);
			mapsSubMenu->Show();
		}
		else if(id == 2)
		{
			debugMenu->ToggleFocus(false);
			RefreshMaps();
			chaptersSubMenu->ToggleFocus(true);
			chaptersSubMenu->Show();
		}
		else if(id == 4) 
		{
			mk::Core::Exit();
		}
	}

	// Selection d'un item du sub menu game states
	id = statesSubMenu->GetPressedItemId();
	if(id != -1)
	{
		if(id < statesSubMenu->GetItemCount() - 1) 
		{
			std::unordered_map<std::string, CGameState*>& statesList = game->GetAvailableStates();
			int cpt = 0;
			for(std::unordered_map<std::string, CGameState*>::iterator it = statesList.begin(); it != statesList.end(); it++)
			{
				if(id == cpt) 
				{
					game->ChangeState((it)->second);
					break;
				}
				cpt++;
			}
			
		}
		else if(id == statesSubMenu->GetItemCount() - 1) 
		{
			debugMenu->ToggleFocus(true);
			statesSubMenu->ToggleFocus(false);
			statesSubMenu->Hide();
		}
	}

	// Selection d'une map à charger
	id = mapsSubMenu->GetPressedItemId();
	if(id != -1)
	{
		if(id < mapsSubMenu->GetItemCount() - 1) 
		{
			std::string path;
			DebugItemButton *but = (DebugItemButton*)(mapsSubMenu->GetSelectedItem());
			path = but->GetLabel();

			game->ChangeState(LevelState::Instance() );
			LevelState::Instance()->LoadMap(path);
			return;
		}
		else if(id == mapsSubMenu->GetItemCount() - 1) 
		{
			debugMenu->ToggleFocus(true);
			mapsSubMenu->ToggleFocus(false);
			mapsSubMenu->Hide();
		}
	}

	// Selection d'un chapitre
	id = chaptersSubMenu->GetPressedItemId();
	if(id != -1)
	{
		if(id < chaptersSubMenu->GetItemCount() - 1) 
		{
			// Chargement d'un chapitre du jeu
			game->ChangeState(ChapterState::Instance() );
			ChapterState::Instance()->SetChapter("chapters/chapter_2.xml");
			return;
		}
		else if(id == chaptersSubMenu->GetItemCount() - 1) 
		{
			debugMenu->ToggleFocus(true);
			chaptersSubMenu->ToggleFocus(false);
			chaptersSubMenu->Hide();
		}
	}
}

void DebugState::Draw(StateManager* game, int mode, float interpolation)
{
	if(mode == mk::MODE_2D)
	{
		// Fond
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), MK_MAKE_RGBA(192, 192, 192, 255) );
		lowDisplayFillRect(0, 0, mk::Core::getBaseWidth(), 32, 0x000000FF);
		lowDisplayFillRect(0, mk::Core::getBaseHeight()-32, mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 0x000000FF);

		// Affichage des menus
		debugMenu->MoveTo(64, 100);
		debugMenu->UpdateAndDraw();

		statesSubMenu->MoveTo(400, 100);
		statesSubMenu->UpdateAndDraw();

		mapsSubMenu->MoveTo(400, 132);
		mapsSubMenu->UpdateAndDraw();

		chaptersSubMenu->MoveTo(400, 164);
		chaptersSubMenu->UpdateAndDraw();
	}
	else if(mode == mk::MODE_3D)
	{

	}
}

void DebugState::RefreshMaps()
{
	// Clear
	mapsSubMenu->ClearMenu();

	// Listing des .tmx dans le dossier maps
	std::vector<std::string>& filenames = listingRecursif("data/maps");
	for(std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); it++) 
	{
		if((*it).find("as") != std::string::npos)
			continue;

		mapsSubMenu->AddItem(new DebugItemButton((*it)));
	}
	mapsSubMenu->AddItem(new DebugItemButton("Retour"));
}
