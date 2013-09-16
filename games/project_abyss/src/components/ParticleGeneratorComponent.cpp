// project_abyss
// LittleSpace Studio 2012

#include "ParticleGeneratorComponent.h"
#include "../entities/Entity.h"


ParticleGeneratorComponent::ParticleGeneratorComponent(std::string filename)
{
	gen = new ParticleGenerator(filename);
}

ParticleGeneratorComponent::~ParticleGeneratorComponent()
{
	delete gen;
}

void ParticleGeneratorComponent::Load(std::string filename)
{
	gen->Load(filename);
}

void ParticleGeneratorComponent::SetArea(float w, float h)
{
	gen->SetArea(w, h);
}

void ParticleGeneratorComponent::Init()
{
	gen->Init(parent->GetScene() );
}

void ParticleGeneratorComponent::ChangePriority(int prio)
{
	gen->ChangePriority(prio);
}

void ParticleGeneratorComponent::SetIgnoreLightPipeline(bool t)
{
	gen->SetIgnoreLightPipeline(t);
}

void ParticleGeneratorComponent::Receive(int message, void* data)
{

}

void ParticleGeneratorComponent::Update()
{
	gen->SetPosition(parent->mPos.x, parent->mPos.y, parent->mDepth);
	gen->Update();
}

void ParticleGeneratorComponent::RotateLocal(float px, float py, float angle)
{
	gen->RotateLocal(px, py, angle);
}

void ParticleGeneratorComponent::MirrorLocal(bool v, bool h)
{
	gen->MirrorLocal(v, h);
}