// project_abyss
// LittleSpace Studio 2013

#ifndef MK_PARTICLES_ENGINE_MANAGER_H
#define MK_PARTICLES_ENGINE_MANAGER_H

#include "mkengine.h"
#include <list>
#include "../effects/ParticleGenerator.h"

#define PARTICLE_ENGINES_COUNT		32

class ParticleEnginesManager
{
	public:
		ParticleEnginesManager(mk::Scene* scene);
		~ParticleEnginesManager();

		void ShowGenerator(std::string filename, float x, float y, float z);
		
		void Update();

	public:
		ObjectPool<ParticleGenerator> * pool;
		std::list<ParticleGenerator*> allocated;
		mk::Scene* scene;
};

#endif