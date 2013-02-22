// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_POINT
#define MK_POINT

namespace mk 
{
	class Point 
	{
	public:
		Point() { x = 0, y = 0, z = 0; };

		float x, y, z;
	};
};

#endif