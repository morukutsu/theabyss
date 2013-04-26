// project_abyss
// LittleSpace Studio 2012

#include "BodyComponent.h"
#include "../physics/Body.h"
#include "../physics/Vector.h"
#include "../physics/Polygon.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

BodyComponent::BodyComponent(int _type, int _numVertices, float _w, float _h)
{
	body = new CBody();
	type = _type;
	numVertices = _numVertices;
	w = _w;
	h = _h;
}

void BodyComponent::Receive(int message, void* data)
{

}

void BodyComponent::Update()
{

}

void BodyComponent::Init()
{
	// Cockpit
	NVector* points;
	switch(type)
	{
		case BODY_CMP_BLOB:
			points = PolyColl::BuildBlob(numVertices, w);
			break;
		case BODY_CMP_BOX:
			points = PolyColl::BuildBox(numVertices, w, h);
			break;
	}

	body->Initialise(parent->mPos, 0.0f, points, numVertices);
	body->SetFriction(0.1f);
	body->SetGlue(0.0f);
	body->bodytype = BODY_FULL;

	parent->GetEntityManager()->GetGameMap()->AddBody(body);

	parent->SetSensorBody(body);
}