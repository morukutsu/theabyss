// project_abyss
// LittleSpace Studio 2012

#ifndef MK_LIGHT_COMPONENT
#define MK_LIGHT_COMPONENT

#include "Component.h"
#include "mkengine.h"

enum { LIGHT_MESH, LIGHT_SPRITE };

class LightComponent : public Component
{
	public:
		LightComponent(float r);
		LightComponent(std::string filename);
		~LightComponent();

		void Receive(int message, void* data);

		void Update();

		void Init();

		void SetAngle(float _angle) { angle = _angle; };
		void Mirror(bool v, bool h) { mirrorV = v, mirrorH = h; };
		void Offset(float x, float y) { offsetX = x, offsetY = y; };

		void SetDepthTest(bool v) { spr.SetDepthTest(v); };
	public:
		float radius;
		mk::DrawableMesh mesh;
		mk::Sprite spr;

		int lightType;

		bool mirrorH, mirrorV;
		float offsetX, offsetY;
		float angle;
};

#endif
