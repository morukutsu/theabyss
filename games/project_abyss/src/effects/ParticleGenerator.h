// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PARTICLE_GENERATOR
#define MK_PARTICLE_GENERATOR

#include "mkengine.h"

#include <vector>
#include <deque>
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

	mk::Sprite spr, light;
};

class ParticleGenerator
{
	public:
		ParticleGenerator();
		ParticleGenerator(std::string filename);
		~ParticleGenerator();

		void Reset();
		void Update();

		void Init(mk::Scene* scene);
		void Load(std::string filename);

		void ChangePriority(int prio);
		void SetIgnoreLightPipeline(bool t);
		void RotateLocal(float x, float y, float angle);
		void MirrorLocal(bool v, bool h);

		void SetArea(float w, float h);

		void SetPosition(float px, float py, float pz) { x = px, y = py, z = pz; };
	
private:
		void GetColorInterp(float life, float *r, float *g, float *b);
		mk::Image* PickSprite();

	public:
		static void HSL2RGB(float h, float sl, float l, float *r, float *g, float *b);
		static void RGB2HSL(float r, float g, float b, float *h, float *s, float *l);
		static void ColorInterpolation(float t, float r1, float g1, float b1, float r2, float g2, float b2, float* tr, float* tg, float* tb);
	
	public:
		// Scene
		mk::Scene* scene;

		// Position
		float x, y, z;

		// Gestion des particules
		std::deque<Particle> particles;
		float particleBootTime, genTime;
		bool isActive, stillAlive;
		float movingParent_vx, movingParent_vy, movingParent_vz;
		int particleCount, emittedCount;
		int numAdded;

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