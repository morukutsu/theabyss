// project_abyss
// LittleSpace Studio 2012

#include "AnimatedGraphicsComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

AnimatedGraphicsComponent::AnimatedGraphicsComponent(std::string modelf, float _scale, int _prio, std::string defaultAnim, float _angle, bool _mirrorX, bool _mirrorY, int _anim_offset, bool _no_shadow)
{
	// Chargement des ressources
	model.Scale(scale, scale);
	model.LoadModelDescriptorFile(modelf);

	if(defaultAnim == "")
		defaultAnim = model.animations[0].name;

	model.PlayAnim(ANIM_LOOP, defaultAnim);

	model.Mirror(false, true);

	model.SetAnimFrame(_anim_offset);

	scale = _scale;
	prio = _prio;
	angle = _angle;
	mirrorX = _mirrorX;
	mirrorY = !_mirrorY;
	anim_offset = _anim_offset;

	no_shadow = _no_shadow;

	offsetX = offsetY = 0;
}

void AnimatedGraphicsComponent::Init()
{
	parent->GetScene()->Add(&model);
	model.ignoreLightPipeline = no_shadow;
}


void AnimatedGraphicsComponent::Update()
{
	model.SetDepth(parent->mDepth);
	model.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y  + offsetY)/32);
	model.Alpha(1.0f);
	model.Rotate(angle);
	model.Mirror(mirrorX, mirrorY);
	model.Scale(scale, scale);
	model.SetPriority(prio);
	//model.Play(1.0f/30.0f);
	model.UpdateTime();
	model.SavePositions();
}

void AnimatedGraphicsComponent::Receive(int message, void* data)
{

}
