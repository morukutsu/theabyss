// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PARTICLE_GENERATOR_COMPONENT
#define MK_PARTICLE_GENERATOR_COMPONENT

#include "Component.h"
#include "mkengine.h"

#include <vector>
#include <math.h>
#include "../SimpleMaths.h"

struct Particle 
{
	bool active;
	float x, y, z;
	float vx, vy, vz;
	float life, fade;
	float alpha, alphaFade;
	float angle, angleVariation;
	float size, sizeVariation;
	float r, g, b;

	mk::Sprite spr;
};

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

	private:
		void GetColorInterp(float life, float *r, float *g, float *b);
		mk::Image* PickSprite();

	public:
		static void HSL2RGB(float h, float sl, float l, float *r, float *g, float *b);
		static void RGB2HSL(float r, float g, float b, float *h, float *s, float *l);

	public:
		// Gestion des particules
		std::vector<Particle> particles;
		float particleBootTime, genTime;
		bool isActive;
		float movingParent_vx, movingParent_vy, movingParent_vz;

		// Paramètres
		mk::ParticleGeneratorRessource* params;

		int priority;
		bool ignoreLightPipeline;

		// Rotation locale
		float rotCenterX, rotCenterY, rotAngle;

		// Mirroing local
		bool mirrorV, mirrorH, oldMirrorV, oldMirrorH;

		// Paramètres pouvant être changés (copiés)
		float areaW, areaH;
		float vx, vy, vz;
		float offsetX, offsetY, offsetZ;
};

#endif