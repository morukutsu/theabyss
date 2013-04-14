// project_abyss
// LittleSpace Studio 2012

#include "SensorComponent.h"
#include "../physics/Body.h"
#include "../physics/Vector.h"
#include "../physics/Polygon.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

SensorComponent::SensorComponent(float w, float h)
{
	sensor = new CBody();
	width = w;
	height = h;
}

void SensorComponent::Init()
{
	// Création du body de la taille de la porte
	int numVerts = 4;
	NVector* points = PolyColl::BuildBox(numVerts, width, height);
	sensor->Initialise(parent->mPos + NVector(width/2, height/2), 0.0f, points, numVerts);
	sensor->isSensor = true;
	sensor->bodytype = BODY_FULL;

	parent->GetEntityManager()->GetGameMap()->AddBody(sensor);
	
	parent->SetSensorBody(sensor);
}

void SensorComponent::Receive(int message, void* data)
{

}

void SensorComponent::Update()
{
}


