// project_abyss
// LittleSpace Studio 2012

#include "EnnemyFishComponent.h"
#include "IABaseEnnemyComponent.h"
#include "ComponentsIncludes.h"
#include "../entities/Entity.h"
#include "../physics/BodyDef.h"
#include "../physics/Body.h"

EnnemyFishComponent::EnnemyFishComponent()
{
	gfx = new GraphicsComponent("anims/mobs/fish/fish_placeholder.png");
	cmp = new IABaseEnnemyComponent();
	body = new BodyComponent(BODY_ENNEMY, BODY_CMP_BLOB, 10, 20.0f, 20.0f);
}

EnnemyFishComponent::~EnnemyFishComponent()
{
	// TODO : supprimer les composants

}

void EnnemyFishComponent::Init()
{
	parent->AddComponent(gfx);
	parent->AddComponent(cmp);
	parent->AddComponent(body);
}

void EnnemyFishComponent::Receive(int message, void* data)
{

}

void EnnemyFishComponent::Update()
{
	// Settings de variables au controleur d'IA
	cmp->SetPlayerSpotted(false);
	cmp->SetPlayerTouched(false);
	cmp->SetLife(100);

	// On effectue un déplacement en fonction de l'état courant
	prevState = state;
	state = cmp->GetState();

	switch(state)
	{
		case IABaseEnnemyComponent::S_WAIT:
			// On ne bouge pas
			body->body->SetLinearVelocity(NVector(0, 0) );
			break;

		case IABaseEnnemyComponent::S_MOVE:
			if(prevState != IABaseEnnemyComponent::S_MOVE)
			{
				// Choisit une direction au hasard
				moveVel.x = SimpleMaths::Rand(-1.0f, 1.0f);
				moveVel.y = SimpleMaths::Rand(-1.0f, 1.0f);
				moveVel.Normalise();

				moveVel = moveVel * 5.0f;
			}
			body->body->SetLinearVelocity(moveVel );
			break;

		case IABaseEnnemyComponent::S_ATTACK:
			std::cout << "attacking" << std::endl;
			break;

		case IABaseEnnemyComponent::S_HIT:
			std::cout << "hit" << std::endl;
			break;

		case IABaseEnnemyComponent::S_DEAD:
			std::cout << "dead" << std::endl;
			break;
	}

	// Déplacements simples
	parent->mPos = body->body->GetPosition();
	parent->mVel = body->body->GetDisplacement();
}

