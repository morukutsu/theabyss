// project_abyss
// LittleSpace Studio 2012

#ifndef MK_WEAPON_BODY_COMPONENT
#define MK_WEAPON_BODY_COMPONENT

#include "Component.h"
#include "mkengine.h"

#define PULSE_LASER_VELOCITY		60.0F

struct PartPositionInfo {
	float x, y, rx, ry;
};

struct WpnAutofire
{
	float rate;		// number of shoots per sec
};

struct WpnInfo {
	std::string wpn_name;
	float shootX, shootY;
	std::string img_arm, img_cannon;
	PartPositionInfo arm_position, cannon_position;
	bool isArmRotationFixed, isCannonRotationFixed;
	WpnAutofire autofire;
};

enum {
	C_POS_NEUTRAL, C_POS_UP, C_POS_DOWN, C_POS_UP_DIAG, C_POS_DOWN_DIAG, C_POS_FREE
};

enum {
	C_MODE_FIXED, C_MODE_FREE
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
		
		void Orient(int dir);
		void OrientMouse(float mscrx, float mscry);

		void ReadWeaponsFromXML();

	public:
		std::vector<WpnInfo> wpns;
		PlayerInputComponent* in;

		float mWpnAngle;

		int mCannonOrientation;

		int mCannonMode; // fixed / free mode

		// Autofire
		float reloadTime;
		bool allowShoot;
};

#endif