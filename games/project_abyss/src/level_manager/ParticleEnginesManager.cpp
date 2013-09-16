// project_abyss
// LittleSpace Studio 2013

#include "ParticleEnginesManager.h"

ParticleEnginesManager::ParticleEnginesManager(mk::Scene* sc)
{
	pool = new ObjectPool<ParticleGenerator>(PARTICLE_ENGINES_COUNT);
	scene = sc;
}

ParticleEnginesManager::~ParticleEnginesManager()
{
	delete pool;
}

void ParticleEnginesManager::ShowGenerator(std::string filename, float x, float y, float z)
{
	ParticleGenerator* gen = pool->NewInstance();
	if(gen != NULL)
	{
		gen->Load(filename);
		gen->Init(scene);
		gen->SetPosition(x, y, z);

		allocated.push_back(gen);
	}
}

void ParticleEnginesManager::Update()
{
	for(std::list<ParticleGenerator*>::iterator it = allocated.begin(); it != allocated.end(); ) 
	{
		if((*it)->isActive) 
		{
			(*it)->Update();
			it++;
		} 
		else 
		{
			pool->FreeInstance((*it));
			allocated.erase(it++);
		}
	}
}

