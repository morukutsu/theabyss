// project_abyss
// LittleSpace Studio 2012

#include "EnnemyBigfishComponent.h"
#include "ComponentsIncludes.h"
#include "../physics/BodyDef.h"
#include "../entities/Entity.h"
#include "../physics/Body.h"
#include "../entities/EntityManager.h"
#include "../level_manager/GameMap.h"
#include "../level_manager/LevelManager.h"
#include "../entities/Messages.h"

EnnemyBigfishComponent::EnnemyBigfishComponent()
{
	texA = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("anims/mobs/bigfish/bigfish2.png");
	texB = (mk::Image*)mk::RessourceManager::getInstance()->LoadRessource("anims/mobs/bigfish/bigfish.png");

	gfx = new AnimatedGraphicsComponent("anims/mobs/bigfish/bigfish.xml", 4.0f, -3);
	halo = new GraphicsComponent("anims/mobs/bigfish/light_normal.png");
	body = new BodyComponent(BODY_ENNEMY, BODY_CMP_BLOB, 10, 120.0f, 120.0f);

	flare = new GraphicsComponent("anims/mobs/bigfish/halo_light.png");

	// Lights
	light = new LightComponent(6.5f);
	light->mesh.SetPriority(-4);

	lightEye = new LightComponent(1.5f);
	lightEye->mesh.SetPriority(-4);

	halo->prio = -4;
	halo->no_shadow = true;

	flare->prio = -4;
	flare->no_shadow = true;

	nextEntityToFollow = NULL;

	state = S_SEEK;

	mirrorH = false;

	// Choix de la bonne texture
	gfx->model.Assign(texA);

	isSpotted = false;

	cutsceneState = 0;

	curHaloColor = 255;
}

void EnnemyBigfishComponent::Receive(int message, void* data)
{

}

void EnnemyBigfishComponent::Update()
{
	// Mirroring
	float mx = 1.0f;
	if(!lockMirror && ((parent->mVel.x < 0.25f) || (parent->mVel.x > 0.25f)) )
		mirrorH = parent->mVel.x < 0;
	if(mirrorH)
		mx = -1.0f;

	gfx->mirrorX = mirrorH;
	//gfx->offsetX = -40*mx;
	gfx->offsetX = 0;

	// Positionnement des éléments graphiques
	md5_joint_t* bone = gfx->model.GetBone(boneID);
	NVector lanternePos = NVector((bone->pos[0]*gfx->model.scaleX*32)*mx, -bone->pos[1]*gfx->model.scaleY*32);
	lanternePos.Rotate(NVector(0, 0), DegreesToRadians(gfx->angle)*-mx );

	light->Offset(lanternePos.x, lanternePos.y);
	halo->Offset(lanternePos.x, lanternePos.y);
	flare->Offset(lanternePos.x, lanternePos.y);

	bone = gfx->model.GetBone(boneID_eye);
	NVector eyePos = NVector((bone->pos[0]*gfx->model.scaleX*32)*mx, -bone->pos[1]*gfx->model.scaleY*32);
	eyePos.Rotate(NVector(0, 0), DegreesToRadians(gfx->angle)*-mx );
	lightEye->Offset(eyePos.x, eyePos.y);

	FollowWaypoints();

	// Déplacements simples
	parent->mPos = body->body->GetPosition();
	parent->mVel = body->body->GetDisplacement();

	body->body->isSensor = true;	// Temporaire, passage en mode sensor
}

void EnnemyBigfishComponent::Init()
{
	//gfx->offsetY = +100;
	gfx->offsetY = 0;
	flare->alpha = 0.06f;

	parent->AddComponent(gfx);
	parent->AddComponent(halo);
	parent->AddComponent(flare);
	parent->AddComponent(body);
	parent->AddComponent(light);
	parent->AddComponent(lightEye);

	boneID = gfx->model.GetBoneID("\"light\"");
	boneID_eye = gfx->model.GetBoneID("\"eye\"");
}

void EnnemyBigfishComponent::FollowWaypoints()
{
	float mx = 1.0f;
	if(mirrorH)
		mx = -1.0f;

	// Check erreurs
	if(parent->waypoints.size() == 0)
		return;

	// Changement d'état
	// Si le héros se rapproche trop, on passe en S_SPOTTED
	NVector heroPos = parent->GetEntityManager()->GetHeroPosition();
	NVector spottedVec = heroPos - parent->mPos;
	
	oldIsSpotted = isSpotted;
	if(spottedVec.Length() < ENN_BIGFISH_ATTACK_RADIUS && state != S_ATTACK) 
	{
		state = S_ATTACK;
	}
	else if(spottedVec.Length() < ENN_BIGFISH_SPOTTED_RADIUS && state != S_SPOTTED && state != S_ATTACK) 
	{
		isSpotted = true;
	}
	else 
	{
		isSpotted = false;
	}

	CheckCollisions();

	// States
	if(state == S_SEEK)
	{
		lockMirror = false;

		Move(ENN_BIGFISH_SPEED);

		// Changement d'état
		NVector dist = (nextEntityToFollow->mPos + NVector(nextEntityToFollow->mWidth, nextEntityToFollow->mHeight)/2.0f - parent->mPos);
		if(dist.Length() < ENN_BIGFISH_SLOWDIST)
			state = S_SLOW;
	}
	else if(state == S_SLOW) 
	{
		Move(ENN_BIGFISH_SPEED / 2.0f);

		if(WaypointReached() )
		{
			// Changement d'état
			state = S_WAIT;
			mWaitTime = ENN_BIGFISH_WAIT;
		}
	}
	else if(state == S_WAIT) 
	{
		lockMirror = true;
		body->body->SetLinearVelocity(NVector(0, 0) );

		mWaitTime -= 1.0f/30.0f;
		if(mWaitTime < 0)
		{
			state = S_SEEK;
		}
	}
	else if(state == S_ATTACK)
	{
		// On lance la cutscene de gameover
		if(cutsceneState == 0)
		{
			// On lance la cutscene
			mirrorH = spottedVec.x < 0;
			lockMirror = true;
			body->body->SetLinearVelocity(NVector(0, 0) );
			LaunchGameoverScene();
			cutsceneState = 1;
			gfx->model.PlayAnim(ANIM_LOOP, "repere", false);
			mWaitTime = 0;

			// Desactivaiton collisions (knockback) du body avec le héros
			// body->body->bodytype = BODY_FULL;
			body->body->isSleeping = true;
		}
		else if(cutsceneState == 1)
		{
			vel = (heroPos - parent->mPos);

			// Calcul de la vitesse de l'ennemi en fonction de la distance
			mAttackSpeed = 4.0f + vel.Length() / 500.0f;

			// Changement progressif de l'angle
			vel.Normalise();

			// On oriente l'ennemi en direction du héros
			float targetAngle;
			if(!mirrorH)
				targetAngle = -(SimpleMaths::GetAngle2Points(0, 0, vel.x, vel.y) + 90 - 15);
			else
				targetAngle =  (SimpleMaths::GetAngle2Points(vel.x, vel.y, 0, 0) + 90 + 15);

			gfx->angle = RadiansToDegrees(Slerp2D(mWaitTime, 0, 0.5f, 0, DegreesToRadians(targetAngle) ) );

			// On attend une demi seconde
			body->body->SetLinearVelocity(NVector(0, 0) );

			mWaitTime += 1.0f/30.0f;

			if(mWaitTime >= 0.5f)
			{
				cutsceneState = 2;
				mWaitTime = 0.0f;
			}
		}
		else if(cutsceneState == 2)
		{
			// Passage à la phase de déplacement vers le héros
			vel = (heroPos - parent->mPos);
			vel.Normalise();
			vel = vel * ENN_BIGFISH_SPEED * mAttackSpeed;

			body->body->SetLinearVelocity(vel);

			// On oriente l'ennemi en direction du héros
			if(!mirrorH)
				gfx->angle = -(SimpleMaths::GetAngle2Points(0, 0, vel.x, vel.y) + 90 - 15);
			else
				gfx->angle =  (SimpleMaths::GetAngle2Points(vel.x, vel.y, 0, 0) + 90 + 15);

			// Quand le héros est atteint on arrête de bouger
			if(spottedVec.Length() < ENN_BIGFISH_KILL_RADIUS)
			{
				cutsceneState = 3;
			}
		}
		else if(cutsceneState == 3)
		{
			body->body->SetLinearVelocity(NVector(0, 0) );

			// On gobe
			gfx->model.PlayAnim(ANIM_LOOP, "gobe", false);

			// Après qqs temps, attaque du bigfish
			mWaitTime += 1.0f/30.0f;
			if(mWaitTime > 0.35f)
			{
				cutsceneState = 4;
				NVector power = NVector(50000.0f*mx, -7000.0f);
				parent->GetEntityManager()->SendMessageToEntity("hero", MSG_POWER, &power);
				parent->GetEntityManager()->SendMessageToEntity("hero", MSG_BODY_ACTIVATE_GRAVITY, NULL);
				parent->GetEntityManager()->SendMessageToEntity("hero", MSG_HERO_KILL, NULL);
				mWaitTime = 0.0f;
			}
		}
		else if(cutsceneState == 4)
		{
			// Retour en idle
			mWaitTime += 1.0f/30.0f;
			if(mWaitTime > 0.5f)
			{
				vel = (nextEntityToFollow->mPos + NVector(nextEntityToFollow->mWidth, nextEntityToFollow->mHeight)/2.0f - parent->mPos);
				vel.Normalise();
				vel = vel * ENN_BIGFISH_SPEED;
				gfx->angle = 0;
				state = S_SEEK;
			}
		}
	}

	// Lissage changement couleur
	if(curHaloColor < targetHaloColor)
		curHaloColor += 8;
	if(curHaloColor > targetHaloColor)
		curHaloColor -= 8;

	if(curHaloColor > 255)
		curHaloColor = 255;

	if(curHaloColor < 96)
		curHaloColor = 96;

	// Changement de textures et d'animations
	if(state == S_ATTACK)
	{
		targetHaloColor = 96;
		gfx->model.Assign(texB);
		halo->spr.Tint(MK_MAKE_RGBA(255, curHaloColor, curHaloColor, 255) );
		//gfx->model.PlayAnim(ANIM_LOOP, "gobe", false);
	}
	else
	{
		if(isSpotted)
		{
			targetHaloColor = 96;
			gfx->model.Assign(texB);
			gfx->model.PlayAnim(ANIM_LOOP, "repere", false);
			if(oldIsSpotted != isSpotted)
				gfx->model.SetAnimFrame(savedFrame);
			halo->spr.Tint(MK_MAKE_RGBA(255, curHaloColor, curHaloColor, 255) );
			savedFrame = gfx->model.GetAnimFrame();
		}
		else
		{
			targetHaloColor = 255;
			gfx->model.Assign(texA);
			gfx->model.PlayAnim(ANIM_LOOP, "recherche", false);
			if(oldIsSpotted != isSpotted)
				gfx->model.SetAnimFrame(savedFrame);
			halo->spr.Tint(MK_MAKE_RGBA(255, curHaloColor, curHaloColor, 255) );
			savedFrame = gfx->model.GetAnimFrame();
		}
	}
}

void EnnemyBigfishComponent::Move(float speed)
{
	// Initialisation du déplacement
	if(nextEntityToFollow == NULL)
	{
		nextEntityToFollow = parent->waypoints[0];
		curWaypointID = 0;

		// Déplacement vers le prochain waypoint
		vel = (nextEntityToFollow->mPos + NVector(nextEntityToFollow->mWidth, nextEntityToFollow->mHeight)/2.0f - parent->mPos);
		vel.Normalise();
		vel = vel * speed;
	}

	body->body->SetLinearVelocity(vel);
}

bool EnnemyBigfishComponent::WaypointReached()
{
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

		return true;
	}

	return false;
}

void EnnemyBigfishComponent::LaunchGameoverScene()
{
	parent->GetEntityManager()->GetGameMap()->GetLevelManager()->LaunchCutscene("cutscenes/gameover_bigfish.xml");
	parent->GetEntityManager()->GetCommonStateVariables()[C_STATE_LOCK_USER_INPUT] = 1;
}

void EnnemyBigfishComponent::CheckCollisions()
{
	// Collisions
	if(body->body->isCollision)
	{
		std::vector<CBody*>& cbodies = body->body->cbodies;
		for(int i = 0; i < cbodies.size(); i++)
		{
			if(cbodies[i]->bodytype == BODY_BULLET)
			{
				// TODO: verifier plus finement les collisions
				// Désactiver cette vérification une fois un stade du jeu passé ou le héros peut tuer l'ennemi
				state = S_ATTACK;
			}
		}
	}
}