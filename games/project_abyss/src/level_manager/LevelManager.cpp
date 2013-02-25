// project_abyss
// LittleSpace Studio 2012

#include "LevelManager.h"
#include "../DebugState.h"
#include "StateManager.h"
#include "GameMap.h"
#include "../entities/EntityManager.h"
#include "../entities/Entity.h"
#include "../effects/ScrollFadeEffect.h"

LevelManager::LevelManager()
{
	Init();

	// Initialise le debug menu
	mk::Font* fnt = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");
	debugMenuTitle = new DebugMenu(fnt);
	debugMenuTitle->AddItem(new DebugItemButton("Debug menu", 0xFFFFFFFF));

	debugMenuFull = new DebugMenu(fnt);
	debugMenuFull->AddItem(new DebugItemButton("Recharger map", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Reset Z Camera", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Fullscreen", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Afficher Light Mask", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Afficher AABBs Renderer", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Afficher Debug Collisions", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("PostFX Shader", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Retour DebugState", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Retour", 0xFFFFFFFF));
	debugMenuFull->ToggleFocus(false);
	debugMenuFull->Hide();

	debugMenuShader = new DebugMenu(fnt);

	// Listing des .tmx dans le dossier maps
#ifdef ABYSS_DEBUG
	std::vector<std::string>& filenames = listingRecursif("data/shaders");
	for(std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); it++) 
	{
		if((*it).find("png") == std::string::npos) {
			debugMenuShader->AddItem(new DebugItemButton((*it), 0xFFFFFFFF));
		}
	}
#endif
	debugMenuShader->AddItem(new DebugItemButton("Retour", 0xFFFFFFFF));

	debugMenuShader->ToggleFocus(false);
	debugMenuShader->Hide();
}

LevelManager::~LevelManager()
{
	// Liberation des menus
	delete debugMenuTitle;
	delete debugMenuFull;
	delete debugMenuShader;

	if(gameMap != NULL)
		delete gameMap;
}

// Réinitialise le LevelManager à un état neutre
void LevelManager::Init()
{
	// Camera
	gameCamera.Reset();

	// Scrolling
	scrollEntity = NULL;

	// Map
	gameMap = NULL;
	loadedMap = "";

	// Triggers
	isGamePaused = isMapLoaded = false;
	isLevelManagerNeutral = true;

	isShowLightMask = isShowAABB = false;

	// Map change
	toChangeMap = toFinishChangeMap = false;
	mMapChangeTimer = 0.0;
	scrollFadeEffect = NULL;

	// Cutscene
	cutscene = NULL;
}

// Calcule une frame du jeu
void LevelManager::Update()
{
	// Changement de map si nécéssaire
	HandleSwitchMap();
	
	// Cutscene
	if(cutscene)
		cutscene->Update();

	// Update physics
	gameMap->UpdatePhysics();

	// Update entités
	gameMap->GetEntityManager()->Update();

	// Debug
	gameCamera.DebugMove();

	// Scrolling
	ComputeScrolling();

	// Transition
	if(scrollFadeEffect)
		scrollFadeEffect->Update();
}

// Affiche une frame du jeu
void LevelManager::Draw(int mode, float interpolation)
{
	if(mode == mk::MODE_3D)
	{
		// Interpolation du scrolling
		InterpolateScrolling(interpolation);
		CullMap(interpolation);
		CullScene(interpolation);

		// Positionnement matrice camera
		gameCamera.Set();

		// Affichage de la scene
		scene->Draw(interpolation);
		scene->FinalDraw();

		// Debug collisions
		if(isShowDebugCollisions)
		{
			lowSetProjectionPerspective(mk::Core::getWindowWidth(), mk::Core::getBaseHeight()*mk::Core::GetScaleFactor());
			gameCamera.Set();

			gameMap->DebugCollisions();
		}
	} 
	else if(mode == mk::MODE_2D) 
	{
		// Transition
		if(scrollFadeEffect)
			scrollFadeEffect->Draw(interpolation);

		// Affichage du GUI
		ShowDebug();
	}
	else if(mode == mk::MODE_2D_BACKGROUND) 
	{
		// Affichage du background de la map
		if(gameMap) 
		{
			//gameMap->DrawBGColor();
			//gameMap->DrawBGImage();
			scene->DrawBackground();
		}
	}
}

// Charge une map
void LevelManager::LoadMap(std::string filename)
{
	// Scene
	scene = new mk::Scene();
	scene->Clear();

	gameMap = new GameMap();
	gameMap->LoadGameMap(filename, scene);
	gameMap->AddMapLayersToScene(scene);

	ResetCameraDepth();

	if(!gameMap->GetIsNoEntities() )
		SetScrollEntity(gameMap->heroEntity);
	else
		SetScrollEntity(gameMap->camEntity);

	loadedMap = filename;
	isMapLoaded = true;

	// Debug
	std::cout << "Map " << filename << " loaded." << std::endl;

	// TEMP, TODO
	cutscene = new Cutscene(this);
	cutscene->Load("cutscenes/intro_chapitre_2.xml");
}

// Décharge la map
void LevelManager::UnloadMap()
{
	delete gameMap;
	delete scene;
	delete cutscene;
	delete scrollFadeEffect;

	isMapLoaded = false;
}

void LevelManager::SwitchMap(std::string filename, std::string door_idf)
{
	UnloadMap();
	Init();
	
	// Scene
	scene = new mk::Scene();
	scene->Clear();

	gameMap = new GameMap();

	// On positionne le héros sur la bonne porte
	gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] = 1;
	gameMap->mSwitchMapDoorIdf = door_idf;

	gameMap->LoadGameMap(filename, scene);
	gameMap->AddMapLayersToScene(scene);

	ResetCameraDepth();
	SetScrollEntity(gameMap->heroEntity);

	loadedMap = filename;
	isMapLoaded = true;

	// Effet de transition
	if(scrollFadeEffect != NULL)
		delete scrollFadeEffect;

	int effectSens;
	switch(gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_DOOR_ORIENTATION])
	{
	case SCROLL_LEFT:
		effectSens = SCROLL_RIGHT;
		break;
	case SCROLL_RIGHT:
		effectSens = SCROLL_LEFT;
		break;
	case SCROLL_UP:
		effectSens = SCROLL_DOWN;
		break;
	case SCROLL_DOWN:
		effectSens = SCROLL_UP;
		break;
	};

	scrollFadeEffect = new ScrollFadeEffect(effectSens, true);
	scrollFadeEffect->Init();
	scrollFadeEffect->StartEffect();

	gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_LOCK_USER_INPUT] = 1;

	// Debug
	std::cout << "Map " << filename << " loaded." << std::endl;
}

void LevelManager::HandleSwitchMap()
{
	// On détecte s'il faut changer de map
	if(!toChangeMap && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_GOING_TROUGH_DOOR] == 1)
		toChangeMap = true;

	if(toChangeMap)
	{
		if(mMapChangeTimer >= 0.0f && scrollFadeEffect == NULL)
		{
			scrollFadeEffect = new ScrollFadeEffect(gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_DOOR_ORIENTATION], false);
			scrollFadeEffect->Init();
			scrollFadeEffect->StartEffect();
		}

		if(mMapChangeTimer >= 1.5f)
		{
			SwitchMap(gameMap->GetEntityManager()->targetMap, gameMap->GetEntityManager()->targetDoor);
		}

		mMapChangeTimer += 1.0f/30.0f;
	}

	// Fin du changement de map
	if(!toFinishChangeMap && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] == 1)
		toFinishChangeMap = true;

	if(toFinishChangeMap)
	{
		if(mMapChangeTimer >= 0.75f)
		{
			gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] = 0;
		}

		if(mMapChangeTimer >= 0.75f && scrollFadeEffect != NULL)
		{
			delete scrollFadeEffect;
			scrollFadeEffect = NULL;

			gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_LOCK_USER_INPUT] = 0;
			gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_GOING_TROUGH_DOOR] = 0;
			toFinishChangeMap = false;
			mMapChangeTimer = 0.0f;
		}

		mMapChangeTimer += 1.0f/30.0f;
	}
}

// Met le jeu en pause
void LevelManager::Pause()
{
	isGamePaused = true;
}

// Retourne à l'execution du jeu
void LevelManager::Resume()
{
	isGamePaused = false;
}

void LevelManager::ResetCameraDepth()
{
	gameCamera.SetDepth(-gameCamera.GetPixelPerfectPlane(gameMap));
}

void LevelManager::SetScrollEntity(Entity* ent)
{
	scrollEntity = ent;
}

void LevelManager::ComputeScrolling()
{
	prevScrollX = scrollX;
	prevScrollY = scrollY;
	scrollX = scrollEntity->mPos.x/32.0f;
	scrollY = scrollEntity->mPos.y/32.0f;
	
	float bX = gameMap->borderX/32.0f;
	float bY = gameMap->borderY/32.0f;
	float bW = gameMap->borderW/32.0f;
	float bH = gameMap->borderH/32.0f;

	if(scrollX <= bX + mk::Core::getBaseWidth()/2.0f/32.0f)
		scrollX = bX + mk::Core::getBaseWidth()/2.0f/32.0f;
	
	if(scrollX >= bX + bW - mk::Core::getBaseWidth()/2.0f/32.0f)
		scrollX = bX + bW - mk::Core::getBaseWidth()/2.0f/32.0f;

	if(scrollY <= bY + mk::Core::getBaseHeight()/2.0f/32.0f)
		scrollY = bY + mk::Core::getBaseHeight()/2.0f/32.0f;
	
	if(scrollY >= bY + bH - mk::Core::getBaseHeight()/2.0f/32.0f)
		scrollY = bY + bH - mk::Core::getBaseHeight()/2.0f/32.0f;
}

void LevelManager::InterpolateScrolling(float dt)
{
	float sX, sY;
	sX = Lerp(dt, 0.0f, 1.0f, prevScrollX, scrollX);
	sY = Lerp(dt, 0.0f, 1.0f, prevScrollY, scrollY);

	gameCamera.MoveTo(sX, sY);
}

void LevelManager::CullMap(float dt)
{
	float sX, sY;
	sX = Lerp(dt, 0.0f, 1.0f, prevScrollX, scrollX);
	sY = Lerp(dt, 0.0f, 1.0f, prevScrollY, scrollY);

	float originX = sX*gameMap->GetMapScale()/gameMap->map.GetTileWidth();
	float originY = sY*gameMap->GetMapScale()/gameMap->map.GetTileHeight();

	float cx = originX - mk::Core::getBaseWidth()/2.0f/gameMap->map.GetTileWidth();
	float cy = originY - mk::Core::getBaseHeight()/2.0f/gameMap->map.GetTileHeight();

	// TODO : calculer la surface visible pour chaque layer directement dans Map.cpp
	gameMap->SetCullArea(cx, cy, 
		mk::Core::getBaseWidth()/gameMap->map.GetTileWidth(),
		mk::Core::getBaseHeight()/gameMap->map.GetTileHeight());
}

void LevelManager::CullScene(float dt)
{
	float sX, sY;
	sX = Lerp(dt, 0.0f, 1.0f, prevScrollX, scrollX);
	sY = Lerp(dt, 0.0f, 1.0f, prevScrollY, scrollY);

	scene->SetCullOrigin(sX, sY);
}

// Affiche des infos de debug
void LevelManager::ShowDebug()
{
	gameCamera.ShowDebugInfo(20, 80);
	scene->ShowDebugInfo(20, 100);

	debugMenuTitle->MoveTo(20, 120);
	debugMenuTitle->UpdateAndDraw();
	debugMenuFull->MoveTo(200, 120);
	debugMenuFull->UpdateAndDraw();
	debugMenuShader->MoveTo(550, 120);
	debugMenuShader->UpdateAndDraw();

	// DebugMenuTitle
	int id = debugMenuTitle->GetPressedItemId();
	if(id != -1)
	{
		if(id == 0) 
		{
			debugMenuTitle->ToggleFocus(false);
			debugMenuFull->ToggleFocus(true);
			debugMenuFull->Show();
		}
	}

	// DebugMenuFull
	id = debugMenuFull->GetPressedItemId();
	if(id != -1)
	{
		if(id == 0) 
		{
			// Reload map
			std::string oldMap = loadedMap;
			
			// TODO Trouver un moyen de vider correctement le manager
			//mk::RessourceManager::getInstance()->Free();

			UnloadMap();
			Init();
			LoadMap(oldMap);

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 1) 
		{
			ResetCameraDepth();

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 2) 
		{
			mk::Core::ToggleFullscreenMode();

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 3) 
		{
			isShowLightMask = !isShowLightMask;
			scene->DebugShowLightMask(isShowLightMask);

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 4) 
		{
			isShowAABB = !isShowAABB;
			scene->DebugShowAABB(isShowAABB);

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 5) 
		{
			isShowDebugCollisions = !isShowDebugCollisions;

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 6) 
		{
			debugMenuFull->ToggleFocus(false);
			debugMenuShader->ToggleFocus(true);
			debugMenuShader->Show();
		}
		else if(id == debugMenuFull->GetItemCount() - 2) 
		{
			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);

			// Retour DebugState
			StateManager::getInstance()->ChangeState(DebugState::Instance() );
		}
		else if(id == debugMenuFull->GetItemCount() - 1) 
		{
			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
	}

	// DebugMenuShader
	id = debugMenuShader->GetPressedItemId();
	if(id != -1)
	{
		if(id == debugMenuShader->GetItemCount() - 1) 
		{
			// Retour
			debugMenuShader->ToggleFocus(false);
			debugMenuShader->Hide();
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else
		{
			// Chargement du shader
			DebugItemButton *but = (DebugItemButton*)(debugMenuShader->GetSelectedItem());
			scene->LoadPostFXShader(but->GetLabel() );
		}
	}
}