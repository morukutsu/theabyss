// project_abyss
// LittleSpace Studio 2012

#include "GameMap.h"
#include "GameMapLayer.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../entities/EntityFactory.h"
#include <sstream>
#include "../ConfigurationManager.h"
#include "../physics/Body.h"
#include "../physics/Polygon.h"
#include "../physics/Vector.h"

GameMap::GameMap()
{
	SetTextureQuality(ConfigurationManager::getInstance()->GetAttributeInt("Graphics_TextureQuality"));
	entityManager = new EntityManager();
}

GameMap::~GameMap()
{
	// Décharger les entités ...
	for(std::list<CBody*>::iterator it = bodies.begin(); it != bodies.end();)
	{
		delete (*it);
		bodies.erase(it++);
	}
	
	delete entityManager;

	for(std::list<GameMapLayer*>::iterator it = layersDrawables.begin(); it != layersDrawables.end(); it++)
	{
		delete (*it);
	}
}

void GameMap::Init()
{
	heroEntity = NULL;
}

void GameMap::CreateHeroEntity()
{
	// Creation de l'entité
	std::map<std::string, std::string> properties;
	Entity* entity = EntityFactory::Create("hero", "hero", 0, 0, 0.0f, 0, 0, properties);
	heroEntity = entity;
}

void GameMap::LoadGameMap(mk::Scene* scene)
{
	// Chargement de la partie GFX de la map
	Load();
	
	// Chargement des entités
	entityManager->SetScene(scene);
	entityManager->SetGameMap(this);

	// Setting des paramètres à la scene
	scene->ToggleLighting(isLighting);
	if(backgroundImage != NULL)
		backgroundImage->Scale(textureScale, textureScale);
	scene->SetBackground(backgroundImage);

	// Chargement des entités

	// Création héros
	if(!isNoEntities)
		CreateHeroEntity(); // initialise heroEntity

	const std::vector<Tmx::ObjectGroup*>& groups = map.GetObjectGroups();
	
	// Itération sur les couches d'objets
	for(int k = 0; k < groups.size(); k++) 
	{
		// Objets
		const std::vector<Tmx::Object*>& objects = groups[k]->GetObjects();

		// On skip le calque si on est en mode no entites
		if(groups[k]->GetName() == "entities" && isNoEntities)
			continue;

		// Calque d'entités
		if(groups[k]->GetName() == "entities" || groups[k]->GetName() == "gfx") 
		{
			// Parcours des objets
			for(int i = 0; i < objects.size(); i++) 
			{
				// Lecture de la profondeur
				float depth = 0.0f;
				std::string depthStr = objects[i]->GetProperties().GetLiteralProperty("depth");
				if(depthStr != "No such property!") {
					std::stringstream ss;
					ss << depthStr;
					ss >> depth;
				}

				// On ne crée pas le héros...
				if(objects[i]->GetName() == "hero")
				{
					if(GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] != 1)
					{
						startHeroX = objects[i]->GetX() + objects[i]->GetWidth() / 2.0f;
						startHeroY = objects[i]->GetY() + objects[i]->GetHeight() / 2.0f;
					}
					continue;
				}
					
				// Creation de l'entité
				Entity* entity = EntityFactory::Create(objects[i]->GetName(), objects[i]->GetType(),
					objects[i]->GetX(), objects[i]->GetY(), depth, objects[i]->GetWidth(), objects[i]->GetHeight(), 
					objects[i]->GetProperties().GetList());

				entity->SetScene(scene);

				SpecialCases(objects[i]->GetName(), objects[i]->GetType(), entity);

				entityManager->Add(entity);

				entity->Init();
			}
		}
		else if(groups[k]->GetName() == "area") 
		{
			// Parcours des area
			for(int i = 0; i < objects.size(); i++) 
			{
				if(objects[i]->GetName() == "borders")
				{
					borderX = objects[i]->GetX();
					borderY = objects[i]->GetY();
					borderW = objects[i]->GetWidth();
					borderH = objects[i]->GetHeight();
				}
			}
		}
		else if(groups[k]->GetName() == "bounds")
		{
			for(int i = 0; i < objects.size(); i++) 
			{
				const Tmx::Polyline* polyline = objects[i]->GetPolyline();
				for(int j = 0; j < polyline->GetNumPoints()-1; j++)
				{
					const Tmx::Point p1 = polyline->GetPoint(j);
					const Tmx::Point p2 = polyline->GetPoint(j+1);

					NVector* points = new NVector[2];
					points[0].x = objects[i]->GetX() + p1.x;
					points[0].y = objects[i]->GetY() + p1.y;
					points[1].x = objects[i]->GetX() + p2.x;
					points[1].y = objects[i]->GetY() + p2.y;

					NVector center = NVector((points[0].x + points[1].x) / 2.0f, 
						(points[0].y + points[1].y) / 2.0f);

					points[0] = points[0] - center;
					points[1] = points[1] - center;

					CBody* body = new CBody();
					body->Initialise(center, 0.0f, points, 2);
					body->SetFriction(0.1f);
					body->SetGlue(0.0f);

					bodies.push_back(body);
				}
			}
		}
	}

	// Finalisation création héros
	if(!isNoEntities) {
		heroEntity->SetScene(scene);
		entityManager->Add(heroEntity);
		heroEntity->mPos.x = startHeroX;
		heroEntity->mPos.y = startHeroY;
		heroEntity->Init();
	}
}

void GameMap::AddMapLayersToScene(mk::Scene* scene)
{
	// Récupérations des couches de la map
	const std::vector<Tmx::Layer*>& layers = map.GetLayers();

	// Itération sur les couches de la map
	for(int k = 0; k < layers.size(); k++) 
	{	
		GameMapLayer* layerTransparents = new GameMapLayer(this, k, false);
		GameMapLayer* layerOpaques = new GameMapLayer(this, k, true);

		scene->Add(layerTransparents);	// Parties transparentes
		scene->Add(layerOpaques);	// Parties opaques

		layersDrawables.push_back(layerTransparents);
		layersDrawables.push_back(layerOpaques);
	}
}

void GameMap::SpecialCases(std::string name, std::string type, Entity* entity)
{
	if(type.find("door") != std::string::npos) 
	{
		// Si le héros sort d'une porte, on le positionne dessus, au lieu de le positionner sur le départ
		if(name == mSwitchMapDoorIdf && GetEntityManager()->GetCommonStateVariables()[C_STATE_EXITING_DOOR] == 1)
		{
			startHeroX = entity->mPos.x + entity->mWidth / 2.0f;
			startHeroY = entity->mPos.y + entity->mHeight / 2.0f;
		}
	}
	else if(type == "cam")
	{
		camEntity = entity;
	}
}

void GameMap::UpdatePhysics()
{
	// Update des body
	for(std::list<CBody*>::iterator it = bodies.begin(); it != bodies.end(); it++)
	{
		// Update à 30FPS
		(*it)->Update(1.0f/30.0f);

		(*it)->ClearCollisionBodies();
	}

	// Gestion des collisions
	// TODO supprimer les tests en double
	for(std::list<CBody*>::iterator body_a = bodies.begin(); body_a != bodies.end(); body_a++)
	{
		for(std::list<CBody*>::iterator body_b = bodies.begin(); body_b != bodies.end(); body_b++)
		{
			if((*body_a) != (*body_b))
			{
				// On ignore les collisions entre body statiques
				if(!(*body_a)->IsUnmovable() || !(*body_b)->IsUnmovable())
				{
					if((*body_a)->Collide(*(*body_b))) 
					{
						// Enregistrement de la collision
						(*body_a)->AddCollisionWithBody((*body_b));
						(*body_b)->AddCollisionWithBody((*body_a));
					}
				}
			}
		}		
	}
}

void GameMap::AddBody(CBody* body)
{
	bodies.push_back(body);
}

void GameMap::DebugCollisions()
{
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);

	for(std::list<CBody*>::iterator it = bodies.begin(); it != bodies.end(); it++)
	{
		NVector center = (*it)->GetPosition();

		glPushMatrix( );
		glLineWidth(1);
		glTranslatef(center.x/32.0f, center.y/32.0f, 0);
		glColor3f (1.0f, 0.0f, 0.0f);

		glBegin (GL_QUADS);
			float point = 0.1f;
			glVertex3f(-point, -point, 0);
			glVertex3f(+point, -point, 0);
			glVertex3f(+point, +point, 0);
			glVertex3f(-point, +point, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);

		if((*it)->m_iNumVertices != 0)
		{
			NVector po = (*it)->m_axVertices[0];

			glVertex2f(po.x/32.0f, po.y/32.0f);

			for(int i = 1; i < (*it)->m_iNumVertices; i++)
			{
				NVector p = (*it)->m_axVertices[i];
				glVertex2f(p.x/32.0f, p.y/32.0f);
			}
			glVertex2f(po.x/32.0f, po.y/32.0f);
		}

		glEnd();
		glPopMatrix( );
	}

	glDisable (GL_BLEND);
	glEnable (GL_DEPTH_TEST);
}