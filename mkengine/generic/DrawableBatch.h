// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_DRAWABLEBATCH
#define MK_DRAWABLEBATCH

#include <list>
#include "Drawable.h"
#include "lowDisplay.h"

namespace mk
{
	class DrawTask
	{
	public:
		DrawTask(Drawable* _elem, float _x, float _y,
		int _cx1, int _cy1, int _cx2, int _cy2,
		float _scaleX, float _scaleY, float _angle, float _alpha,
		int _centerX, int _centerY,
		float _handleX, float _handleY,
		unsigned char _r, unsigned char _g, unsigned char _b, int _depth)
		{
			elem = _elem;
			x = _x;
			y = _y;
			cx1 = _cx1;
			cy1 = _cy1;
			cx2 = _cx2;
			cy2 = _cy2;
			scaleX = _scaleX;
			scaleY = _scaleY;
			angle = _angle;
			alpha = _alpha;
			centerX = _centerX;
			centerY = _centerY;
			r = _r;
			g = _g;
			b = _b;
			depth = _depth;
		}

		bool operator<(const DrawTask& a)
		{
			return depth < a.depth;
		}

		Drawable* elem;

		float x, y;
		int cx1, cy1, cx2, cy2;
		float scaleX, scaleY, angle, alpha;
		int centerX, centerY;
		float handleX, handleY;
		unsigned char r, g, b;

		int depth;
	};

	class DrawableBatch
	{
		public:
			DrawableBatch();
			void Begin();
			void End();

			void Add(Drawable* elem);
			void Draw();

		public:
			std::list<DrawTask> mDrawElements;
	};
};

#endif