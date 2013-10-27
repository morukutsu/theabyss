// project_abyss
// LittleSpace Studio 2012

#include "LevelManager.h"
#include "../DebugState.h"
#include "StateManager.h"
#include "GameMap.h"
#include "../entities/EntityManager.h"
#include "../entities/Entity.h"
#include "../effects/ScrollFadeEffect.h"
#include "../components/PlayerInputComponent.h"
#include "../script/ScriptManager.h"
#include "../LevelState.h"

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
	debugMenuFull->AddItem(new DebugItemButton("Activer Glow", 0xFFFFFFFF));
	debugMenuFull->AddItem(new DebugItemButton("Activer Lighting", 0xFFFFFFFF));
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
	isShowDebugCollisions = false;

	// Map change
	toChangeMap = toFinishChangeMap = false;
	mMapChangeTimer = 0.0;
	scrollFadeEffect = NULL;

	// Cutscene
	cutscene = NULL;

	scrollDiffX = scrollDiffY = 0;
	scrollDiffDeplX = scrollDiffDeplY = 0;

	scrollingResumeTime = 0;

	// changement de pointeur de souris
	mk::Core::SetPointer((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("gui/aim_cursor.png") );
	mk::Core::TogglePointer(true);

	// Effets
	mMousePositionTimer = 0.0f;
	isEffetTrainee = true;

	// Scripts
	ScriptManager::getInstance()->Init();
}

// Calcule une frame du jeu
void LevelManager::Update()
{
	// Changement de map si nécéssaire
	HandleSwitchMap();
	
	// Cutscene
	if(cutscene)
		cutscene->Update();

	// Update script map
	if(gameMap->GetScriptFilename() != "")
		ScriptManager::getInstance()->ExecuteScriptFunction("void main()");

	// Update physics
	gameMap->UpdatePhysics();

	// Update entités
	gameMap->GetEntityManager()->Update();

	// Update particles
	gameMap->particleManager->Update();

	// Debug
	//gameCamera.DebugMove();

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
		// Effet souris
		AccumulateMousePositions();
		DrawMouseTrainee();

		// Transition
		if(scrollFadeEffect)
			scrollFadeEffect->Draw(interpolation);

		// Affichage du GUI
		if(mk::Core::GetDebugVisible() )
			ShowDebug();

		// Cutscene draw
		if(cutscene)
			cutscene->Draw(interpolation);
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


// Effets
void LevelManager::AccumulateMousePositions()
{
	mMousePositionTimer += lowGetFrameTime();
	if(mMousePositionTimer >= 1.0f/60.0f)
	{
		// Récupération de la position de la souris
		NVector mousePos = NVector(mk::Core::interpPointerX, mk::Core::interpPointerY);

		// Ajout à la liste
		mousePositions.push_front(mousePos);

		if(mousePositions.size() > 6)
			mousePositions.pop_back();
		mMousePositionTimer = 0.0f;
	}
}

void LevelManager::DrawMouseTrainee()
{
	if(!isEffetTrainee)
		return;

	NVector prev1, prev_pa;
	int count = 0;
	bool first = true;

	glEnable(GL_BLEND);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(80/255.0f, 190/255.0f, 213/255.0f, 1.0f);
	for(std::list<NVector>::iterator it = mousePositions.begin(); it != mousePositions.end(); it++)
	{
		// On calcule l'angle des deux points suivants
		NVector pa, pb;
		if(count > 0)
		{
			pa = (*it);
			pa.Normalise();
			pa *= 4.0f;
			pa.Rotate(NVector(0, 0), PI/2.0f);
			pa += (*it);

			pb = (*it);
			pb.Normalise();
			pb *= 4.0f;
			pb.Rotate(NVector(0, 0), -PI/2.0f);
			pb += (*it);
		}

		// Cas ou on a les deux premiers points
		if(count == 1)
		{
			glVertex2f(prev1.x, prev1.y);
			glVertex2f(pa.x, pa.y);
			glVertex2f(pb.x, pb.y);
		}
		else if(count > 1)
		{
			glVertex2f(pa.x, pa.y);
			glVertex2f(pb.x, pb.y);

			float alpha = 1.0f - ((float)count / (float)mousePositions.size());
			glColor4f(80/255.0f, 190/255.0f, 213/255.0f, alpha);
		}

		// Iteration suivante
		first = false;
		prev_pa = pb;
		prev1 = (*it);
		count++;
	}

	glEnd();
	glDisable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

// Charge une map
void LevelManager::LoadMap(std::string filename)
{
	Init();

	// Scene
	scene = new mk::Scene();
	scene->Clear();

	gameMap = new GameMap(this);
	gameMap->ParseMap(filename);
	gameMap->LoadGameMap(scene);
	gameMap->AddMapLayersToScene(scene);

	// Try to load map script if exists
	std::string script = gameMap->GetScriptFilename();
	if(script != "") 
	{
		RegisterFunctionsForScript();
		ScriptManager::getInstance()->LoadScript(script);
		ScriptManager::getInstance()->Build();
		ScriptManager::getInstance()->ExecuteScriptFunction("void init()");
	}

	ResetCameraDepth();

	if(!gameMap->GetIsNoEntities() )
		SetScrollEntity(gameMap->heroEntity);
	else
		SetScrollEntity(gameMap->camEntity);

	loadedMap = filename;
	isMapLoaded = true;

	// Debug
	std::cout << "Map " << filename << " loaded." << std::endl;

	// Launch cutscene
	if(gameMap->cutsceneFilename != "")
		LaunchCutscene(gameMap->cutsceneFilename);

	mk::Core::SetLoadingFrame(true);
}

// Décharge la map
void LevelManager::UnloadMap()
{
	delete gameMap;
	delete scene;
	delete cutscene;
	delete scrollFadeEffect;

	gameMap = NULL;

	isMapLoaded = false;

	ScriptManager::getInstance()->Clean();

	asIScriptEngine* engine = ScriptManager::getInstance()->GetEngine();
	int r = engine->RemoveConfigGroup("runtime_config");
	if(r == asCONFIG_GROUP_IS_IN_USE)
		lowError("cannot remove group");
}

void LevelManager::SwitchMap(std::string filename, std::string door_idf)
{
	// On récupère la liste des tilesets/textures courantes
	std::list<std::string> previousTextures = gameMap->GetResourcesToLoad();

	// Sauvegarde du mirroring du héros
	int heroMirrorH = gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR];

	UnloadMap();
	Init();
	
	// Scene
	scene = new mk::Scene();
	scene->Clear();

	gameMap = new GameMap(this);

	// Resetting du mirroring du héros
	gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MIRROR] = heroMirrorH;

	// On positionne le héros sur la bonne porte
	gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] = 1;
	gameMap->mSwitchMapDoorIdf = door_idf;

	gameMap->ParseMap(filename);

	// On récupère la liste des textures courantes
	std::list<std::string> currentTextures = gameMap->GetResourcesToLoad();

	// On libère de la mémoires les textures qui ne seront pas utilisées dans la prochaine map
	// Phase de recherche
	std::list<std::string> notFoundResources;
	for(std::list<std::string>::iterator it = previousTextures.begin(); it != previousTextures.end(); it++)
	{
		bool found = false;
		for(std::list<std::string>::iterator it2 = currentTextures.begin(); it2 != currentTextures.end(); it2++)
		{
			if((*it) == (*it2))
			{
				found = true;
				break;
			}
		}

		if(!found)
			notFoundResources.push_back((*it));
	}

	// Libération
	for(std::list<std::string>::iterator it = notFoundResources.begin(); it != notFoundResources.end(); it++)
	{
		std::cout << "[Free] Deleting unused tileset/texture " << (*it) << std::endl;
		mk::RessourceManager::getInstance()->DeleteRessource((*it));
	}

	gameMap->LoadGameMap(scene);
	gameMap->AddMapLayersToScene(scene);

	// Try to load map script if exists
	std::string script = gameMap->GetScriptFilename();
	if(script != "") 
	{
		RegisterFunctionsForScript();
		ScriptManager::getInstance()->LoadScript(script);
		ScriptManager::getInstance()->Build();
		ScriptManager::getInstance()->ExecuteScriptFunction("void init()");
	}

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

	mk::Core::SetLoadingFrame(true);

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
	// Différence dynamique

	// Détection de mouvement
	if(gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MOVED] != -1)
		scrollingResumeTime = 0;
	else
		scrollingResumeTime += 1.0f/30.0f;

	// Pour les X
	if(scrollEntity->mVel.x < 0 && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MOVED] == CMD_ACCEL_LEFT)
	{
		scrollDiffDeplX -= DYNAMIC_SCROLLING_SPEED;
	}
	else if(scrollEntity->mVel.x > 0 && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MOVED] == CMD_ACCEL_RIGHT)
	{
		scrollDiffDeplX += DYNAMIC_SCROLLING_SPEED;
	}
	else
	{
		if(scrollingResumeTime > 1.5f)
		{
			if(fabs(scrollDiffDeplX) > DYNAMIC_SCROLLING_SPEED) 
			{
				if(scrollDiffDeplX > 0)
					scrollDiffDeplX -= DYNAMIC_SCROLLING_SPEED;
				else if(scrollDiffDeplX < 0)
					scrollDiffDeplX += DYNAMIC_SCROLLING_SPEED;
			}
			else
			{
				scrollDiffDeplX = 0;
			}
		}
	}

	if(scrollDiffDeplX >= 200/32.0f)
		scrollDiffDeplX = 200/32.0f; 
	if(scrollDiffDeplX <= -200/32.0f)
		scrollDiffDeplX = -200/32.0f;

	// Pour les Y
	if(scrollEntity->mVel.y < 0 && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MOVED] == CMD_ACCEL_UP)
	{
		scrollDiffDeplY -= DYNAMIC_SCROLLING_SPEED;
	}
	else if(scrollEntity->mVel.y > 0 && gameMap->GetEntityManager()->GetCommonStateVariables()[C_STATE_PLAYER_MOVED] == CMD_ACCEL_DOWN)
	{
		scrollDiffDeplY += DYNAMIC_SCROLLING_SPEED;
	}
	else
	{
		if(scrollingResumeTime > 1.5f)
		{
			if(fabs(scrollDiffDeplY) > DYNAMIC_SCROLLING_SPEED) 
			{
				if(scrollDiffDeplY > 0)
					scrollDiffDeplY -= DYNAMIC_SCROLLING_SPEED;
				else if(scrollDiffDeplY < 0)
					scrollDiffDeplY += DYNAMIC_SCROLLING_SPEED;
			}
			else
			{
				scrollDiffDeplY = 0;
			}
		}
	}

	if(scrollDiffDeplY >= 200/32.0f * 9.0f/16.0f)
		scrollDiffDeplY = 200/32.0f * 9.0f/16.0f; 
	if(scrollDiffDeplY <= -200/32.0f * 9.0f/16.0f)
		scrollDiffDeplY = -200/32.0f * 9.0f/16.0f;

	// Scroll diff mouse
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);
	NVector screenCenter = NVector(mk::Core::getBaseWidth()/2.0f, mk::Core::getBaseHeight()/2.0f);
	NVector mouseScreenPosition = NVector(input->pointer.x, input->pointer.y);
	NVector mouseVec = screenCenter - mouseScreenPosition;

	float scrollDiffMouseX, scrollDiffMouseY;

	scrollDiffMouseX = -(mouseVec.x / 32.0f) / 8.0f;
	scrollDiffMouseY = -(mouseVec.y / 32.0f) / 8.0f;

	scrollDiffDeplX = scrollDiffDeplY = 0;

	//scrollDiffX = mk::max(scrollDiffMouseX, scrollDiffDeplX);
	//scrollDiffY = mk::max(scrollDiffMouseY, scrollDiffDeplY);

	// Foce only mouse scroll: TODO : toggle that by option
	if(scrollEntity == gameMap->heroEntity)
	{
		scrollDiffX = scrollDiffMouseX;
		scrollDiffY = scrollDiffMouseY;
	} else {
		scrollDiffX = scrollDiffY = 0;
	}

	if(scrollDiffX >= 200/32.0f)
		scrollDiffX = 200/32.0f; 
	if(scrollDiffX <= -200/32.0f)
		scrollDiffX = -200/32.0f;

	if(scrollDiffY >= 200/32.0f * 9.0f/16.0f)
		scrollDiffY = 200/32.0f * 9.0f/16.0f; 
	if(scrollDiffY <= -200/32.0f * 9.0f/16.0f)
		scrollDiffY = -200/32.0f * 9.0f/16.0f;

	// Setting scrolling
	prevScrollX = scrollX;
	prevScrollY = scrollY;
	scrollX = scrollEntity->mPos.x/32.0f + scrollDiffX;
	scrollY = scrollEntity->mPos.y/32.0f + scrollDiffY;
	
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

void LevelManager::RegisterFunctionsForScript()
{
	asIScriptEngine* engine = ScriptManager::getInstance()->GetEngine();
	engine->BeginConfigGroup("runtime_config");

	// Le pointeur void* ne marche pas ... voir dans angelscript 
	int r;
	//r = engine->RegisterGlobalFunction("void SendMessageToEntity(std::string, int, void*)", asMETHOD(EntityManager, SendMessageToEntity), asCALL_THISCALL_ASGLOBAL, this);
	//if(r < 0)
	//	lowError("error registering function");

	r = engine->RegisterGlobalFunction("int checkCollisionBetweenEntities(const string &in, const string &in)", asMETHOD(EntityManager, checkCollisionBetweenEntities), asCALL_THISCALL_ASGLOBAL, gameMap->GetEntityManager() );
	r = engine->RegisterGlobalFunction("void setEntityPosition(const string &in, float, float)", asMETHOD(EntityManager, setEntityPosition), asCALL_THISCALL_ASGLOBAL, gameMap->GetEntityManager() );
	r = engine->RegisterGlobalFunction("float getEntityPositionX(const string &in)", asMETHOD(EntityManager, getEntityPositionX), asCALL_THISCALL_ASGLOBAL, gameMap->GetEntityManager() );
	r = engine->RegisterGlobalFunction("float getEntityPositionY(const string &in)", asMETHOD(EntityManager, getEntityPositionY), asCALL_THISCALL_ASGLOBAL, gameMap->GetEntityManager() );
	r = engine->RegisterGlobalFunction("void addWaypointToEntity(const string &in, const string &in)", asMETHOD(EntityManager, addWaypointToEntity), asCALL_THISCALL_ASGLOBAL, gameMap->GetEntityManager() );

	/*if(r < 0)
	{
		switch(r)
		{
		case asNOT_SUPPORTED:
			printf("asNOT_SUPPORTED\n");
			break;
		case asWRONG_CALLING_CONV:
			printf("asWRONG_CALLING_CONV\n");
			break;
		case asINVALID_DECLARATION:
			printf("asINVALID_DECLARATION\n");
			break;
		case asNAME_TAKEN:
			printf("asNAME_TAKEN\n");
			break;
		case asALREADY_REGISTERED:
			printf("asALREADY_REGISTERED\n");
			break;
		};
		lowError("error registering function");
	}*/

	engine->EndConfigGroup();
}

// Cutscenes
void LevelManager::LaunchCutscene(std::string path)
{
	if(cutscene)
		delete cutscene;

	cutscene = new Cutscene(this);
	cutscene->Load(path);
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
			scene->isEffectBloom = !scene->isEffectBloom;

			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == 7) 
		{
			scene->isLighting = !scene->isLighting;
			
			// Retour
			debugMenuFull->ToggleFocus(false);
			debugMenuFull->Hide();
			debugMenuTitle->ToggleFocus(true);
		}
		else if(id == debugMenuFull->GetItemCount() - 3)  
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
			//UnloadMap();
			//StateManager::getInstance()->ChangeState(DebugState::Instance() );
			
			// Il ne faut plus jamais updater l'état courant après un change state
			//return;
			LevelState::Instance()->returnToDebugState = true;
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