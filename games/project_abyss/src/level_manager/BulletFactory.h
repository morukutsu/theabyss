// project_abyss
// LittleSpace Studio 2012

#ifndef MK_BULLET_FACTORY
#define MK_BULLET_FACTORY

enum {
	BULLET_PULSE_LASER
};

class Bullet;
class BulletFactory 
{
	public:
		static void Init(int bullet_type, Bullet* bullet);
};

#endif