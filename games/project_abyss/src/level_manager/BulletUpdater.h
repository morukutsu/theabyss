// project_abyss
// LittleSpace Studio 2012

#ifndef MK_BULLET_UPDATER
#define MK_BULLET_UPDATER

#define PULSE_LASER_MAX_SPEED			100.0F

class Bullet;
class BulletUpdater
{
	public:
		static void Update(Bullet* bullet);
};

#endif