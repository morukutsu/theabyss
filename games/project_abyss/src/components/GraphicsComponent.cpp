// project_abyss
// LittleSpace Studio 2012

#include "GraphicsComponent.h"
#include <iostream>
#include "../entities/Entity.h"
#include "RessourceManager.h"

GraphicsComponent::GraphicsComponent(std::string filename, float _scale, int _prio, float _angle, bool _mirrorX, bool _mirrorY, bool _no_shadow)
{
	spr.Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(filename.c_str() ) );
	spr.Set3DMode(true);

	scale = _scale;
	prio = _prio;
	angle = _angle;
	mirrorX = _mirrorX;
	mirrorY = _mirrorY;

	no_shadow = _no_shadow;

	offsetX = offsetY = 0;

	alpha = 1.0f;
}

void GraphicsComponent::Init()
{
	parent->GetScene()->Add(&spr);
	spr.ignoreLightPipeline = no_shadow;

	// test
	spr.SetDepth(parent->mDepth);
	spr.SetSize(spr.image->getImageWidth()/32.0f, spr.image->getImageHeight()/32.0f);
	spr.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y + offsetY)/32);
	//spr.Scale(scale, scale);
	spr.Rotate(angle);
	spr.Mirror(mirrorX, mirrorY);
	spr.SetPriority(prio);
	spr.Alpha(alpha);
	spr.SavePositions();
}

GraphicsComponent::~GraphicsComponent()
{
	parent->GetScene()->Remove(&spr);
}


void GraphicsComponent::Update()
{
	spr.SetDepth(parent->mDepth);
	spr.SetSize(spr.image->getImageWidth()/32.0f, spr.image->getImageHeight()/32.0f);
	spr.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y + offsetY)/32);
	//spr.Scale(scale, scale);
	spr.Rotate(angle);
	spr.Mirror(mirrorX, mirrorY);
	spr.SetPriority(prio);
	spr.Alpha(alpha);
	spr.SavePositions();
}

void GraphicsComponent::Receive(int message, void* data)
{

}
