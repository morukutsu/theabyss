// project_abyss
// LittleSpace Studio 2012

#include "SpineAnimatedGraphicsComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

SpineAnimatedGraphicsComponent::SpineAnimatedGraphicsComponent(std::string modelf, float _scale, int _prio, std::string defaultAnim, float _angle, bool _mirrorX, bool _mirrorY, int _anim_offset, bool _no_shadow)
{
	// Chargement des ressources
	model.Scale(_scale/32.0f, _scale/32.0f);
	model.LoadModelDescriptorFile(modelf);

	if(defaultAnim == "")
		defaultAnim = model.skeletonData->animations[0]->name;

	model.PlayAnim(defaultAnim, ANIM_LOOP);

	model.Mirror(false, true);

	//model.SetAnimFrame(_anim_offset);

	scale = _scale;
	prio = _prio;
	angle = _angle;
	mirrorX = _mirrorX;
	mirrorY = !_mirrorY;
	anim_offset = _anim_offset;

	no_shadow = _no_shadow;

	offsetX = offsetY = 0;
}

void SpineAnimatedGraphicsComponent::Init()
{
	parent->GetScene()->Add(&model);
	model.ignoreLightPipeline = no_shadow;
}

SpineAnimatedGraphicsComponent::~SpineAnimatedGraphicsComponent()
{
	parent->GetScene()->Remove(&model);
}

void SpineAnimatedGraphicsComponent::Update()
{
	model.SetDepth(parent->mDepth);
	model.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y  + offsetY)/32);
	//model.Alpha(1.0f);
	model.Rotate(angle);
	model.Mirror(mirrorX, mirrorY);
	model.Scale(scale, scale);
	model.SetPriority(prio);
	model.Play();
	model.SavePositions();
}

void SpineAnimatedGraphicsComponent::Receive(int message, void* data)
{

}
