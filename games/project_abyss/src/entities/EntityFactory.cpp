// project_abyss
// LittleSpace Studio 2012

#include "EntityFactory.h"
#include "Entity.h"

#include "EntitiesIncludes.h"
#include "../components/ComponentsIncludes.h"
#include "../physics/BodyDef.h"

Entity* EntityFactory::Create(std::string name, std::string type, 
	float x, float y, float z, float w, float h, std::map<std::string, std::string>& properties,
	float lifetime)
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
		float angle = 0.0f;
		bool mirrorX = false;
		bool mirrorY = false;

		if(properties.find("angle") != properties.end() )
			angle = toFloat(properties["angle"]);

		if(properties.find("mirror_x") != properties.end() )
			mirrorX = toInt(properties["mirror_x"]) == 1;

		if(properties.find("mirror_y") != properties.end() )
			mirrorY = toInt(properties["mirror_y"]) == 1;

		ent = new StaticEntity();
		LightComponent* c = new LightComponent(toFloat(properties["radius"]) );
		c->Mirror(mirrorX, mirrorY);
		c->SetAngle(angle);

		ent->AddComponent(c);
	}
	else if(type == "lightimg")
	{
		float angle = 0.0f;
		bool mirrorX = false;
		bool mirrorY = false;

		if(properties.find("angle") != properties.end() )
			angle = toFloat(properties["angle"]);

		if(properties.find("mirror_x") != properties.end() )
			mirrorX = toInt(properties["mirror_x"]) == 1;

		if(properties.find("mirror_y") != properties.end() )
			mirrorY = toInt(properties["mirror_y"]) == 1;

		ent = new StaticEntity();

		LightComponent* c = new LightComponent(properties["image"]);
		c->Mirror(mirrorX, mirrorY);
		c->SetAngle(angle);

		ent->AddComponent(c);
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

		PlayerWeaponComponent* pwpn = new PlayerWeaponComponent(input);
		ent->AddComponent(pwpn);

		ent->AddComponent(new ShipDisplayComponent(pmov, pwpn) );

		
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
		float angle = 0.0f;
		bool mirrorX = false;
		bool mirrorY = false;
		int anim_offset = 0;
		bool no_shadow = false;

		if(properties.find("scale") != properties.end() )
			scale = toFloat(properties["scale"]);

		if(properties.find("angle") != properties.end() )
			angle = toFloat(properties["angle"]);

		if(properties.find("prio") != properties.end() )
			prio = toInt(properties["prio"]);

		if(properties.find("defaultanim") != properties.end() )
			defaultanim = properties["defaultanim"];

		if(properties.find("mirror_x") != properties.end() )
			mirrorX = toInt(properties["mirror_x"]) == 1;

		if(properties.find("mirror_y") != properties.end() )
			mirrorY = toInt(properties["mirror_y"]) == 1;

		if(properties.find("anim_offset") != properties.end() )
			anim_offset = toInt(properties["anim_offset"]);

		if(properties.find("no_shadow") != properties.end() )
			no_shadow = toInt(properties["no_shadow"]) == 1;

		ent = new StaticEntity();
		ent->AddComponent(new AnimatedGraphicsComponent(properties["model"], scale, prio, defaultanim, angle, mirrorX, mirrorY, anim_offset, no_shadow) );
	}
	else if(type == "gfx")
	{
		float scale = 1.0f;
		int prio = 0;
		float angle = 0.0f;
		bool mirrorX = false;
		bool mirrorY = false;
		bool no_shadow = false;

		if(properties.find("scale") != properties.end() )
			scale = toFloat(properties["scale"]);

		if(properties.find("prio") != properties.end() )
			prio = toInt(properties["prio"]);

		if(properties.find("angle") != properties.end() )
			angle = toFloat(properties["angle"]);

		if(properties.find("mirror_x") != properties.end() )
			mirrorX = toInt(properties["mirror_x"]) == 1;

		if(properties.find("mirror_y") != properties.end() )
			mirrorY = toInt(properties["mirror_y"]) == 1;

		if(properties.find("no_shadow") != properties.end() )
			no_shadow = toInt(properties["no_shadow"]) == 1;

		ent = new StaticEntity();
		ent->AddComponent(new GraphicsComponent(properties["image"], scale, prio, angle, mirrorX, mirrorY, no_shadow) );
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
	else if(type == "sensor")
	{
		ent = new StaticEntity();
		ent->AddComponent(new SensorComponent(w, h) );
	}
	else if(type == "ennemy_bigfish")
	{
		ent = new MoveableEntity();
		ent->AddComponent(new EnnemyBigfishComponent() );
	}
	else if(type == "waypoint")
	{
		ent = new StaticEntity();
	}
	else
	{
		lowError("Uknown entity/gfx type : " + type);
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