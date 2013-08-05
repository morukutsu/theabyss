// project_abyss
// LittleSpace Studio 2012

#include "EnnemyFishComponent.h"
#include "IABaseEnnemyComponent.h"
#include "ComponentsIncludes.h"
#include "../entities/Entity.h"
#include "../physics/BodyDef.h"
#include "../physics/Body.h"
#include "../entities/EntityManager.h"
#include "../level_manager/LevelManager.h"

EnnemyFishComponent::EnnemyFishComponent()
{
	gfx = new AnimatedGraphicsComponent("anims/mobs/fish/fish.xml", 2.0f, -3);
	cmp = new IABaseEnnemyComponent();
	body = new BodyComponent(BODY_ENNEMY, BODY_CMP_BLOB, 10, 50.0f, 50.0f);
	light = new LightComponent(10.0f);
	light->mesh.SetPriority(-4);

	cmp->SetPlayerTouched(false);
	cmp->SetPlayerSpotted(false);

	life = 30;
	cmp->SetLife(life);

	deadTime = 0.0f;

	progressiveSpeed = 1.0f;
	targetSpeed = 1.0f;
}

EnnemyFishComponent::~EnnemyFishComponent()
{

}

void EnnemyFishComponent::Init()
{
	parent->AddComponent(gfx);
	parent->AddComponent(cmp);
	parent->AddComponent(body);
	parent->AddComponent(light);
}

void EnnemyFishComponent::Receive(int message, void* data)
{

}

void EnnemyFishComponent::Update()
{
	// Settings de variables au controleur d'IA
	LookForPlayer();
	CheckCollisionsWithBullets();

	// On effectue un déplacement en fonction de l'état courant
	prevState = state;
	state = cmp->GetState();

	NVector heroPos = parent->GetEntityManager()->GetHeroPosition();

	switch(state)
	{
		case IABaseEnnemyComponent::S_WAIT:
			// On ne bouge pas
			body->body->SetLinearVelocity(body->body->GetDisplacement() * 0.95f);
			progressiveSpeed = 1.0f;
			gfx->model.PlayAnim(ANIM_LOOP, "idle");
			break;

		case IABaseEnnemyComponent::S_MOVE:
			if(prevState != IABaseEnnemyComponent::S_MOVE)
			{
				// Choisit une direction au hasard
				moveVel.x = SimpleMaths::Rand(-1.0f, 1.0f);
				moveVel.y = SimpleMaths::Rand(-1.0f, 1.0f);
				moveVel.Normalise();

				targetSpeed = 5.0f;
			}

			body->body->SetLinearVelocity(moveVel * progressiveSpeed);
			gfx->model.PlayAnim(ANIM_LOOP, "move");
			break;

		case IABaseEnnemyComponent::S_ATTACK:
			{
			NVector spottedVec = heroPos - parent->mPos;
			spottedVec.Normalise();

			targetSpeed = 15.0f;
			moveVel = spottedVec * progressiveSpeed;
			body->body->SetLinearVelocity(moveVel );
			}
			gfx->model.PlayAnim(ANIM_LOOP, "attack");
			break;

		case IABaseEnnemyComponent::S_HIT:
			if(prevState != IABaseEnnemyComponent::S_HIT)
			{
				body->body->SetLinearVelocity(body->body->GetDisplacement() * 0.95f);
				
				life -= 10;
				cmp->SetLife(life);
			}
			gfx->model.PlayAnim(ANIM_LOOP, "idle");
			break;

		case IABaseEnnemyComponent::S_DEAD:
			body->body->SetLinearVelocity(NVector(0, 0) );
			deadTime += 1.0f/30.0f;
			if(deadTime > 2.0f)
			{
				parent->toDelete = true;
				body->body->toDelete = true;
			}
			gfx->model.PlayAnim(ANIM_LOOP, "idle");
			break;
	}

	// Mise à jour de la vitesse
	if(progressiveSpeed > targetSpeed) 
	{
		progressiveSpeed -= 0.5f;
	}
	else if(progressiveSpeed < targetSpeed) 
	{
		progressiveSpeed += 0.5f;
	}

	// Mirroring
	float mx = 1.0f;
	if(((parent->mVel.x < 0.25f) || (parent->mVel.x > 0.25f)) )
		mirrorH = parent->mVel.x < 0;
	if(mirrorH)
		mx = -1.0f;

	gfx->mirrorX = !mirrorH;

	// Déplacements simples
	parent->mPos = body->body->GetPosition();
	parent->mVel = body->body->GetDisplacement();
}

void EnnemyFishComponent::LookForPlayer()
{
	NVector heroPos = parent->GetEntityManager()->GetHeroPosition();
	NVector spottedVec = heroPos - parent->mPos;
	
	if(spottedVec.Length() < ENNEMY_FISH_ATTACK_RADIUS) 
	{
		cmp->SetPlayerSpotted(true);
	}
	else
	{
		cmp->SetPlayerSpotted(false);
	}
}

void EnnemyFishComponent::CheckCollisionsWithBullets()
{
	// Collisions
	if(body->body->isCollision)
	{
		std::vector<CBody*>& cbodies = body->body->cbodies;
		for(int i = 0; i < cbodies.size(); i++)
		{
			if(cbodies[i]->bodytype == BODY_BULLET && cmp->GetState() != IABaseEnnemyComponent::S_HIT)
			{
				cmp->SetPlayerTouched(true);
			}
		}
	}
	else
	{
		cmp->SetPlayerTouched(false);
	}
}