// project_abyss
// LittleSpace Studio 2012

#ifndef MK_BODY_COMPONENT
#define MK_BODY_COMPONENT

#include "Component.h"
#include "mkengine.h"

enum {
	BODY_CMP_BOX, BODY_CMP_BLOB
};

class CBody;
class BodyComponent : public Component
{
	public:
		BodyComponent(int bodydef, int type, int numVertices, float w, float h = 0.0f);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		// Hitbox "solides"
		CBody* body;

		int bodydef, type, numVertices;
		float w, h;
};

#endif