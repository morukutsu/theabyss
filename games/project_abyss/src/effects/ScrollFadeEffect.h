// project_abyss
// LittleSpace Studio 2012

#ifndef MK_EFFECT_SCROLLFADE
#define MK_EFFECT_SCROLLFADE

#include "Effect.h"

enum {
	SCROLL_LEFT, SCROLL_RIGHT,
	SCROLL_UP, SCROLL_DOWN
};

#define GRADIENT_SIZE 256

class ScrollFadeEffect : public Effect
{
	public:
		ScrollFadeEffect(int kind, bool inout);
		~ScrollFadeEffect();
		
		void Init();
		void Update();
		void Draw(float it);
		void StartEffect();
		void StopEffect();

	private:
		int mKind;
		float mXDir, mYDir;
		float mOriginX, mOriginY, mScrollX, mScrollY;
		float mSpeed;
		float oldScrollX, oldScrollY;
		bool mInOut;
};

#endif