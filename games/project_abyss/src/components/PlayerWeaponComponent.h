// project_abyss
// LittleSpace Studio 2012

#ifndef MK_WEAPON_BODY_COMPONENT
#define MK_WEAPON_BODY_COMPONENT

#include "Component.h"
#include "mkengine.h"

#define PULSE_LASER_VELOCITY		40.0F


struct WpnInfo {
	std::string wpn_name;
	float shootX, shootY;
};

class CBody;
class PlayerInputComponent;
class PlayerWeaponComponent : public Component
{
	public:
		PlayerWeaponComponent(PlayerInputComponent* in);

		void Receive(int message, void* data);

		void Update();

		void Init();

		void Shoot();

		void ReadWeaponsFromXML();

	public:
		std::vector<WpnInfo> wpns;
		PlayerInputComponent* in;
};

#endif