// project_abyss
// LittleSpace Studio 2012

#ifndef MK_SENSOR_COMPONENT
#define MK_SENSOR_COMPONENT

#include "Component.h"
#include "mkengine.h"
#include "../entities/Messages.h"

class CBody;
class SensorComponent : public Component
{
	public:
		SensorComponent(float w, float h);

		void Receive(int message, void* data);

		void Update();

		void Init();

	public:
		float width, height;
		CBody* sensor;
};

#endif
