// project_abyss
// LittleSpace Studio 2012

#include "BodyComponent.h"
#include "../physics/Body.h"
#include "../physics/Vector.h"
#include "../physics/Polygon.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

BodyComponent::BodyComponent(int _bodydef, int _type, int _numVertices, float _w, float _h)
{
	body = new CBody();
	bodydef = _bodydef;
	type = _type;
	numVertices = _numVertices;
	w = _w;
	h = _h;
}

BodyComponent::~BodyComponent()
{
	// supprimer le body du monde
	parent->GetEntityManager()->GetGameMap()->RemoveBody(body);
	body->toDelete = true;
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
		default:
			lowError("Type de body inconnu, BodyComponent");
			break;
	}

	body->Initialise(parent->mPos, 1.0f, points, numVertices);
	body->SetFriction(0.1f);
	body->SetGlue(0.0f);
	body->bodytype = bodydef;

	parent->GetEntityManager()->GetGameMap()->AddBody(body);

	parent->SetSensorBody(body);
}