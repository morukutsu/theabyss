// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_SCENE
#define MK_SCENE

#include <list>
#include "Drawable.h"
#include "FBO.h"
#include "Shader.h"
#include "lowDisplay.h"

#define BLACK_BARS_HEIGHT		100

namespace mk
{
	class Sprite;
	class Scene
	{
		public:
			Scene();
			~Scene();

			void Draw(float dt);
			void DrawBackground();
			void FinalDraw();

			void FrustumCulling();
			void Interpolation(float dt);
			void LightPass(std::list<Drawable*>& list);
			void DiffusePass(std::list<Drawable*>& list);
			void StencilWritePass(std::list<Drawable*>& list);

			void ApplyLights();
			void SplitElementsList(std::list<Drawable*>& opaques, std::list<Drawable*>& trasparents);

			void ShowDebugInfo(float x, float y);
			void Clear();

			int  GetNodesCount() { return elements.size(); };

			// Nodes management
			void Add(Drawable* elem);
			void Remove(Drawable* elem);

			void AddLight(Drawable* elem);
			void RemoveLight(Drawable* elem);

			// Pipeline configuration
			void ToggleLighting(bool v) { isLighting = v; };
			void LoadPostFXShader(std::string shader);

			// Culling
			void SetCullOrigin(float _cx, float _cy);
			BoundingBox TransformWindowAABB(float z);

			void SetBackground(mk::Sprite* bg);

			// Special effets
			void InitBlur();
			void RenderBlur(float intensity);

			void InitBlackBands();
			void RenderBlackBands(float interp);
			void ActivateBlackBands(bool toggle);
			void UpdateBlackBands();

			// Debug rendering pipeline
			void DebugShowLightMask(bool v) { isShowLightMask = v; };
			void DebugShowAABB(bool v) { isShowAABB = v; };
			void DebugDrawWindowAABB();
		public:
			// Liste générale
			std::list<Drawable*> elements;

			// Pipeline de rendu
			bool isLighting, isPostFXShader;
			FBO *mWorkFBO;
			Shader postfx;
			Shader light_alpha_fix;

			// Frustum culling
			mk::BoundingBox windowAABB;
			float cullOriginX, cullOriginY;
			int mDisplayedElements;

			// Background
			mk::Sprite *backgroundSprite;
			
			// Gestion de quelques effets graphiques
			Shader shader_blurH, shader_blurV;
			float blur_intensity;
			bool isEffectBlur;
			float black_bands_h, old_black_bands_h, black_bands_time;
			bool isBlackBandsActivated, isBlackBandsDisplayed, blackbandsSens;

			// Debug
			bool isShowLightMask, isShowAABB;
			mk::Font* debugFont;
	};
};

#endif