// project_abyss
// LittleSpace Studio 2012

#ifndef H_LEVELMANAGER
#define H_LEVELMANAGER

#include "mkengine.h"
#include <string>
#include "GameMap.h"
#include "GameCamera.h"
#include "debug_tools\DebugMenu.h"
#include "debug_tools\FileLister.h"
#include "Cutscene.h"
#include "../physics/Vector.h"

#define DYNAMIC_SCROLLING_SPEED 0.15F

class Entity;
class ScrollFadeEffect;
class LevelManager 
{
public:
	LevelManager();
	~LevelManager();

	void Init();								// R�initialise le LevelManager � un �tat neutre
	void Update();								// Calcule une frame du jeu
	void Draw(int mode, float interpolation);	// Affiche une frame du jeu
	void Pause();								// Met le jeu en pause
	void Resume();								// Retourne � l'execution du jeu

	void LoadMap(std::string filename);			// Charge une map
	void UnloadMap();							// D�charge la map
		
	void SwitchMap(std::string filename, std::string door_idf);		// Change de map (passage de porte par ex)
	void HandleSwitchMap();

	// Camera
	void ResetCameraDepth();

	// Scrolling
	void SetScrollEntity(Entity* ent);
	void ComputeScrolling();
	void InterpolateScrolling(float dt);
	void CullMap(float dt);
	void CullScene(float dt);

	// Script
	void RegisterFunctionsForScript();

	// Cutscenes
	void LaunchCutscene(std::string path);

	// Effets
	void AccumulateMousePositions();
	void DrawMouseTrainee();

private:
	void ShowDebug();

public:
	// Triggers
	bool isGamePaused, isMapLoaded, isLevelManagerNeutral;

	// Scene Manager
	mk::Scene* scene;

	// Map
	GameMap* gameMap;
	std::string loadedMap;

	// Ressources
	std::vector<mk::Ressource*> mapResources; 

	// Camera
	GameCamera gameCamera;

	// Scrolling
	Entity* scrollEntity;
	float scrollX, scrollY, prevScrollX, prevScrollY, scrollDiffX, scrollDiffY;
	float scrollingResumeTime;
	float scrollDiffDeplX, scrollDiffDeplY;

	// Cutscenes
	Cutscene* cutscene;

	// Map change
	bool toChangeMap, toFinishChangeMap;
	float mMapChangeTimer;
	ScrollFadeEffect* scrollFadeEffect;

	// Effet pointeur
	std::list<NVector> mousePositions;
	float mMousePositionTimer;
	bool isEffetTrainee;

	// Debug
	DebugMenu *debugMenuTitle, *debugMenuFull, *debugMenuShader;
	bool isShowLightMask, isShowAABB, isShowDebugCollisions, isDebugMenuVisible;
};

#endif
