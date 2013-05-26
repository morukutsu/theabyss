// project_abyss
// LittleSpace Studio 2012

#ifndef MK_ENTITYFACTORY
#define MK_ENTITYFACTORY

#include <map>
#include <string>
#include <sstream>

class Entity;
class EntityFactory
{
	public:
		static Entity* Create(std::string name, std::string type, 
			float x, float y, float z, float w, float h,
			std::map<std::string, std::string>& properties,
			float lifetime = -1.0f);

		static int toInt(std::string v);
		static float toFloat(std::string v);
};

#endif