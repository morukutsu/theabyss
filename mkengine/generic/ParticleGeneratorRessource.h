// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_PGEN_RES
#define MK_PGEN_RES

#include "Ressource.h"
#include "mkengine.h"

#define MAX_PARAMETERS			16

namespace mk 
{
	struct GenSprite
	{
		float frequency;
		mk::Image* img;
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
			return (min + ((float) rand() / RAND_MAX * (max - min )));
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

	struct ColorKey
	{
		float time;
		float r, g, b;
	};

	RangeFunc* createFunc(std::string name);

	enum 
	{
		PARAM_VEL_X, PARAM_VEL_Y, PARAM_VEL_Z
	};

	enum
	{
		AREA_RECT
	};

	class ParticleGeneratorRessource : public Ressource
	{
		public:
			ParticleGeneratorRessource();
			~ParticleGeneratorRessource();

			int LoadFromFile(const char* filename);	
			int LoadFromFile(FILE* fp, long size);		
			int LoadFromMemory(char* mem, int size);

		private:
			void Load(const char* buffer);

		public:
			int maxParticles;
			std::vector<GenSprite> imgs;

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

			bool referencial;

			bool isAreaGenerator;
			int  areaType;

			float areaW, areaH;
			float areaZ_min, areaZ_max;

			int limitParticles, emitAmount;

			// Fonctions
			RangeFunc* rangeFuncs[MAX_PARAMETERS];

			std::vector<ColorKey> colorKeys;
	};
};

#endif MK_PGEN_RES