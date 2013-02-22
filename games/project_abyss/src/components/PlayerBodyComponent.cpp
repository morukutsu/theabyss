// project_abyss
// LittleSpace Studio 2012

#include "PlayerBodyComponent.h"
#include "../physics/Body.h"
#include "../physics/Vector.h"
#include "../physics/Polygon.h"
#include "../entities/Entity.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"

PlayerBodyComponent::PlayerBodyComponent()
{
	cockpit = new CBody();
}

void PlayerBodyComponent::Receive(int message, void* data)
{

}

void PlayerBodyComponent::Update()
{

}

void PlayerBodyComponent::Init()
{
	// Cockpit
	NVector* points = PolyColl::BuildBlob(8, 40.0f);
	cockpit->Initialise(parent->mPos, 1.0f, points, 8);
	cockpit->SetFriction(0.1f);
	cockpit->SetGlue(0.0f);
	cockpit->bodytype = BODY_HERO;

	parent->GetEntityManager()->GetGameMap()->AddBody(cockpit);
}