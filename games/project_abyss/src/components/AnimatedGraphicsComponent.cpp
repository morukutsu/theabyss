// project_abyss
// LittleSpace Studio 2012

#include "AnimatedGraphicsComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

AnimatedGraphicsComponent::AnimatedGraphicsComponent(std::string modelf, float _scale, int _prio, std::string defaultAnim)
{
	// Chargement des ressources
	model.Scale(scale, scale);
	model.LoadModelDescriptorFile(modelf);

	model.PlayAnim(ANIM_LOOP, defaultAnim);

	model.Mirror(false, true);

	scale = _scale;
	prio = _prio;
}

void AnimatedGraphicsComponent::Init()
{
	parent->GetScene()->Add(&model);
}


void AnimatedGraphicsComponent::Update()
{
	model.SetDepth(parent->mDepth);
	model.MoveTo(parent->mPos.x/32, parent->mPos.y/32);
	model.Alpha(1.0f);
	model.Scale(scale, scale);
	model.SetPriority(prio);
	model.Play(1.0f/30.0f);
	model.SavePositions();
}

void AnimatedGraphicsComponent::Receive(int message, void* data)
{

}
