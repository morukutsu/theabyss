// project_abyss
// LittleSpace Studio 2012

#include "EntityFactory.h"
#include "Entity.h"

#include "EntitiesIncludes.h"
#include "../components/ComponentsIncludes.h"

Entity* EntityFactory::Create(std::string name, std::string type, 
	float x, float y, float z, float w, float h, std::map<std::string, std::string>& properties)
{
	// Vérifications
	if(name == "")
		lowError("Une entité dans la map n'a pas de nom.");

	if(type == "")
		lowError("L'entite " + name + " a son type qui n'est pas rempli.");

	// Types d'entités
	Entity* ent = NULL;

	// Debug ouput
	std::cout << "++ Entity: '" << name << "' is > " << type << std::endl;

	if(type == "light")
	{
		ent = new StaticEntity();
		ent->AddComponent(new LightComponent(toFloat(properties["radius"]) ) );
	}
	else if(type == "lightimg")
	{
		ent = new StaticEntity();
		ent->AddComponent(new LightComponent(properties["image"]) );
	}
	else if(type == "hero")
	{
		ent = new MoveableEntity();
		PlayerInputComponent* input = new PlayerInputComponent();
		ent->AddComponent(input);

		PlayerBodyComponent* body = new PlayerBodyComponent();
		ent->AddComponent(body);

		PlayerMovementComponent* pmov = new PlayerMovementComponent(input, body);
		ent->AddComponent(pmov);
		ent->AddComponent(new ShipDisplayComponent(pmov) );
	}
	else if(type == "particlegenerator")
	{
		ent = new StaticEntity();
		ent->AddComponent(new ParticleGeneratorComponent(properties["filename"]) );
	}
	else if(type == "animatedgfx")
	{
		float scale = 1.0f;
		int prio = 0;
		std::string defaultanim = "idle";

		if(properties.find("scale") != properties.end() )
			scale = toFloat(properties["scale"]);

		if(properties.find("prio") != properties.end() )
			prio = toInt(properties["prio"]);

		if(properties.find("defaultanim") != properties.end() )
			defaultanim = properties["defaultanim"];

		ent = new StaticEntity();
		ent->AddComponent(new AnimatedGraphicsComponent(properties["model"], scale, prio, defaultanim) );
	}
	else if(type == "gfx")
	{
		float scale = 1.0f;
		int prio = 0;

		if(properties.find("scale") != properties.end() )
			scale = toFloat(properties["scale"]);

		if(properties.find("prio") != properties.end() )
			prio = toInt(properties["prio"]);

		ent = new StaticEntity();
		ent->AddComponent(new GraphicsComponent(properties["image"], scale, prio) );
	}
	else if(type == "door_free") 
	{
		ent = new StaticEntity();
		ent->AddComponent(new DoorComponent(properties["sens"], w, h, properties["target_map"], properties["target_door"]));
	}
	else if(type == "cam")
	{
		ent = new StaticEntity();
	}

	// Parametres de base
	ent->mPos.x = x;
	ent->mPos.y = y;
	ent->mDepth = z;
	ent->mWidth = w;
	ent->mHeight = h;

	ent->mName = name;
	ent->mType = type;

	return ent;
}

int EntityFactory::toInt(std::string v_str)
{
	int v;
	std::stringstream ss;
	ss << v_str;
	ss >> v;

	return v;
}

float EntityFactory::toFloat(std::string v_str)
{
	float v;
	std::stringstream ss;
	ss << v_str;
	ss >> v;

	return v;
}