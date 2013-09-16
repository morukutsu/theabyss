// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PARTICLE_GENERATOR_COMPONENT
#define MK_PARTICLE_GENERATOR_COMPONENT

#include "Component.h"
#include "../effects/ParticleGenerator.h"

class ParticleGeneratorComponent : public Component
{
	public:
		ParticleGeneratorComponent(std::string filename);
		~ParticleGeneratorComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();
		void Load(std::string filename);

		void ChangePriority(int prio);
		void SetIgnoreLightPipeline(bool t);
		void RotateLocal(float x, float y, float angle);
		void MirrorLocal(bool v, bool h);

		void SetArea(float w, float h);

	public:
		ParticleGenerator* gen;
};

#endif