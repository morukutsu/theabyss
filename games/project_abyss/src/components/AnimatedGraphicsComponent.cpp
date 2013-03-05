// project_abyss
// LittleSpace Studio 2012

#include "AnimatedGraphicsComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

AnimatedGraphicsComponent::AnimatedGraphicsComponent(std::string modelf, float _scale, int _prio, std::string defaultAnim, float _angle, bool _mirrorX, bool _mirrorY, int _anim_offset)
{
	// Chargement des ressources
	model.Scale(scale, scale);
	model.LoadModelDescriptorFile(modelf);

	model.PlayAnim(ANIM_LOOP, defaultAnim);

	model.Mirror(false, true);

	model.SetAnimFrame(_anim_offset);

	scale = _scale;
	prio = _prio;
	angle = _angle;
	mirrorX = _mirrorX;
	mirrorY = !_mirrorY;
	anim_offset = _anim_offset;
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
	model.Rotate(angle);
	model.Mirror(mirrorX, mirrorY);
	model.Scale(scale, scale);
	model.SetPriority(prio);
	model.Play(1.0f/30.0f);
	model.SavePositions();
}

void AnimatedGraphicsComponent::Receive(int message, void* data)
{

}
