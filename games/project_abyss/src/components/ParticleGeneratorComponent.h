// project_abyss
// LittleSpace Studio 2012

#ifndef MK_PARTICLE_GENERATOR_COMPONENT
#define MK_PARTICLE_GENERATOR_COMPONENT

#include "Component.h"
#include "mkengine.h"

#include <vector>
#include <math.h>
#include "../SimpleMaths.h"

#define MAX_PARAMETERS			16

enum 
{
	PARAM_VEL_X, PARAM_VEL_Y, PARAM_VEL_Z
};

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

class RangeFunc
{
	public:
	virtual float func(float t, float min, float max) = 0;
};

class RangeFuncRandom : public RangeFunc
{
	public:
	float func(float t, float min, float max) {
		return SimpleMaths::Rand(min, max);
	}
};

class RangeFuncSin : public RangeFunc
{
	public:
	float func(float t, float min, float max) {
		float v = (1.0f + sinf(t))/2.0f;	// Sin entre 0 et 1
		v = v * (max-min);	// Scale des valeurs dans l'intervalle min/max
		v = v + min;		// Ajout de la borne min
		return v;
	}
};

RangeFunc* createFunc(std::string name);

class ParticleGeneratorComponent : public Component
{
	public:
		ParticleGeneratorComponent(std::string filename);
		~ParticleGeneratorComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void ChangePriority(int prio);
		void SetIgnoreLightPipeline(bool t);
		void RotateLocal(float x, float y, float angle);
		void MirrorLocal(bool v, bool h);

	public:
		// Gestion des particules
		std::vector<Particle> particles;
		float particleBootTime, genTime;
		bool isActive;

		// Paramètres
		int maxParticles;
		std::string spriteFilename;
		mk::Image* img;

		float initialLife;					// Durée de vie en secondes de la particule
		float initialSize, sizeVariation;	// Scale initial de la particule
		float frequency;					// Nombre de particules par seconde
		float gravity;						// Gravité
		float fade, fade_rmin, fade_rmax;	// Facteur de disparition de la particule
		float vx, vy, vz;					// Vitesse de la particule
		float vx_rmin, vy_rmin, vz_rmin;
		float vx_rmax, vy_rmax, vz_rmax;
		float initialAlpha;
		float alphaFade;
		float initialAngle, angleVariation;
		float initialColorR, initialColorG, initialColorB;

		float offsetX, offsetY, offsetZ;

		float movingParent_vx, movingParent_vy, movingParent_vz;
		bool referencial;
		int priority;
		bool ignoreLightPipeline;

		// Fonctions
		RangeFunc* rangeFuncs[MAX_PARAMETERS];

		// Rotation locale
		float rotCenterX, rotCenterY, rotAngle;

		// Mirroing local
		bool mirrorV, mirrorH, oldMirrorV, oldMirrorH;
};

#endif