// project_abyss
// LittleSpace Studio 2012

#include "EnnemyBigfishComponent.h"
#include "ComponentsIncludes.h"
#include "../physics/BodyDef.h"
#include "../entities/Entity.h"
#include "../physics/Body.h"

EnnemyBigfishComponent::EnnemyBigfishComponent()
{
	gfx = new AnimatedGraphicsComponent("anims/mobs/bigfish/bigfish.xml", 4.0f, -3);
	halo = new GraphicsComponent("anims/mobs/bigfish/light_normal.png");
	body = new BodyComponent(BODY_ENNEMY, BODY_CMP_BLOB, 10, 120.0f, 120.0f);

	// Lights
	light = new LightComponent(6.5f);
	light->mesh.SetPriority(-4);

	lightEye = new LightComponent(1.5f);
	lightEye->mesh.SetPriority(-4);

	halo->prio = -4;
	halo->no_shadow = true;

	nextEntityToFollow = NULL;
}

void EnnemyBigfishComponent::Receive(int message, void* data)
{

}

void EnnemyBigfishComponent::Update()
{
	// Mirroring
	bool mirrorH = false;
	float mx = 1.0f;
	if((parent->mVel.x < 0.1f) || (parent->mVel.x > 0.1f) )
		mirrorH = parent->mVel.x < 0;
	if(mirrorH)
		mx = -1.0f;

	gfx->mirrorX = mirrorH;
	gfx->offsetX = -40*mx;

	// Positionnement des éléments graphiques
	md5_joint_t* bone = gfx->model.GetBone(boneID);
	light->Offset(-40*mx + (bone->pos[0]*gfx->model.scaleX*32)*mx, 100 - bone->pos[1]*gfx->model.scaleY*32);
	halo->Offset(-40*mx + (bone->pos[0]*gfx->model.scaleX*32)*mx, 100 - bone->pos[1]*gfx->model.scaleY*32);

	bone = gfx->model.GetBone(boneID_eye);
	lightEye->Offset(-40*mx + (bone->pos[0]*gfx->model.scaleX*32)*mx, 100 - bone->pos[1]*gfx->model.scaleY*32);

	// Déplacements simples
	FollowWaypoints();

	parent->mPos = body->body->GetPosition();
	parent->mVel = body->body->GetDisplacement();
}

void EnnemyBigfishComponent::Init()
{
	
	gfx->offsetY = +100;

	parent->AddComponent(gfx);
	parent->AddComponent(halo);
	parent->AddComponent(body);
	parent->AddComponent(light);
	parent->AddComponent(lightEye);

	boneID = gfx->model.GetBoneID("\"light\"");
	boneID_eye = gfx->model.GetBoneID("\"eye\"");
}

void EnnemyBigfishComponent::FollowWaypoints()
{
	// Check erreurs
	if(parent->waypoints.size() == 0)
		return;

	// Initialisation du déplacement
	if(nextEntityToFollow == NULL)
	{
		nextEntityToFollow = parent->waypoints[0];
		curWaypointID = 0;

		// Déplacement vers le prochain waypoint
		vel = (nextEntityToFollow->mPos + NVector(nextEntityToFollow->mWidth, nextEntityToFollow->mHeight)/2.0f - parent->mPos);
		vel.Normalise();
		vel = vel * ENN_BIGFISH_SPEED;
	}

	body->body->SetLinearVelocity(vel);

	// Check si l'ennemi a atteint le waypoint
	if(parent->mPos.x > nextEntityToFollow->mPos.x 
		&& parent->mPos.x < nextEntityToFollow->mPos.x + nextEntityToFollow->mWidth
		&& parent->mPos.y > nextEntityToFollow->mPos.y 
		&& parent->mPos.y < nextEntityToFollow->mPos.y + nextEntityToFollow->mHeight)
	{
		curWaypointID++;
		curWaypointID = curWaypointID % parent->waypoints.size();
		nextEntityToFollow = parent->waypoints[curWaypointID];

		// Déplacement vers le prochain waypoint
		vel = (nextEntityToFollow->mPos + NVector(nextEntityToFollow->mWidth, nextEntityToFollow->mHeight)/2.0f - parent->mPos);
		vel.Normalise();
		vel = vel * ENN_BIGFISH_SPEED;
	}
}