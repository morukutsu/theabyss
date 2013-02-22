// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_COLOR
#define MK_COLOR

namespace mk 
{
	class Color 
	{
	public:
		Color() { r = 0, g = 0, b = 0, a = 1; };

		float r, g, b, a;
	};
};

#endif