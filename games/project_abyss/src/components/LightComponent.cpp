// project_abyss
// LittleSpace Studio 2012

#include "LightComponent.h"
#include <iostream>
#include "../entities/Entity.h"

LightComponent::LightComponent(float r)
{
	radius = r;

	// Création du mesh de la light
	mesh.SetVertexMode(MK_TRIANGLE_FAN);

	int precision = 32;
	float angle = 0.0f;
	float diff = PI/(float)precision;

	mk::Vertex center, prev;
	center.color.r = center.color.g = center.color.b = 1.0f;
	center.point.x = 0.0f;
	center.point.y = 0.0f;
	center.point.z = 0.0f;
	center.color.a = 1.0f;
	mesh.AddVertex(center);

	for(int k = 1; k < precision*2+2; k++)
	{
		mk::Vertex cur;
		center.color.r = center.color.g = center.color.b = 0.0f;
		cur.point.x = cosf(angle)*radius;
		cur.point.y = sinf(angle)*radius;
		cur.point.z = 0.0f;

		mesh.AddVertex(cur);

		angle += diff;
	}

	lightType = LIGHT_MESH;

	offsetX = offsetY = 0;
	mirrorH = mirrorV = false;
	angle = 0;
	mesh.SetPriority(-1);
}

LightComponent::LightComponent(std::string filename)
{
	spr.Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(filename.c_str() ) );
	spr.Set3DMode(true);
	spr.SetPriority(-1);

	lightType = LIGHT_SPRITE;

	offsetX = offsetY = 0;
	mirrorH = mirrorV = false;
	angle = 0;
}

LightComponent::~LightComponent()
{
	// Ajout de la light au scene node
	if(lightType == LIGHT_MESH)
		parent->GetScene()->RemoveLight(&mesh);
	else if(lightType == LIGHT_SPRITE)
		parent->GetScene()->RemoveLight(&spr);
}

void LightComponent::Init()
{
	// Ajout de la light au scene node
	if(lightType == LIGHT_MESH)
		parent->GetScene()->AddLight(&mesh);
	else if(lightType == LIGHT_SPRITE)
		parent->GetScene()->AddLight(&spr);
}


void LightComponent::Update()
{
	if(lightType == LIGHT_MESH)
	{
		mesh.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y + offsetY)/32, parent->mDepth);
		mesh.SavePositions();
	}
	else if(lightType == LIGHT_SPRITE)
	{
		spr.Mirror(mirrorV, mirrorH);
		spr.SetSize(spr.image->getImageWidth()/32.0f, spr.image->getImageHeight()/32.0f);
		spr.MoveTo((parent->mPos.x + offsetX)/32, (parent->mPos.y + offsetY)/32);
		spr.SetDepth(parent->mDepth);
		spr.Rotate(angle);
		spr.SavePositions();
	}
	
}

void LightComponent::Receive(int message, void* data)
{

}
