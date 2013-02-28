// project_abyss
// LittleSpace Studio 2012

#include "ScrollFadeEffect.h"
#include "mkengine.h"
#include "../SimpleMaths.h"

ScrollFadeEffect::ScrollFadeEffect(int kind, bool inout) : Effect()
{
	mKind = kind;
	mInOut = inout;
}

ScrollFadeEffect::~ScrollFadeEffect()
{
}
		
void ScrollFadeEffect::Init()
{
	// IN
	switch(mKind)
	{
		case SCROLL_LEFT:
			mOriginX = mScrollX = -mk::Core::getBaseWidth();
			mOriginY = mScrollY = mk::Core::getBaseHeight();
			mXDir = 1.0f;
			mYDir = 0.0f;
			break;
		case SCROLL_RIGHT:
			mOriginX = mScrollX = mk::Core::getBaseWidth();
			mOriginY = mScrollY = mk::Core::getBaseHeight();
			mXDir = -1.0f;
			mYDir = 0.0f;
			break;
		case SCROLL_UP:
			mOriginX = mScrollX = mk::Core::getBaseWidth();
			mOriginY = mScrollY = -mk::Core::getBaseHeight();
			mXDir = 0.0f;
			mYDir = 1.0f;
			break;
		case SCROLL_DOWN:
			mOriginX = mScrollX = mk::Core::getBaseWidth();
			mOriginY = mScrollY = mk::Core::getBaseHeight();
			mXDir = 0.0f;
			mYDir = -1.0f;
			break;
	};
}

void ScrollFadeEffect::Update()
{
	if(!isActive)
		return;

	mTimer += 1.0f/30.0f;

	mSpeed = mTimer*mTimer*350;

	oldScrollX = mScrollX;
	oldScrollY = mScrollY;

	switch(mKind)
	{
		case SCROLL_LEFT:
		case SCROLL_RIGHT:
			mScrollX = mScrollX + mXDir*mSpeed;
			if(mXDir > 0 && mScrollX > GRADIENT_SIZE)
				mScrollX = GRADIENT_SIZE;
			else if(mXDir < 0 && mScrollX < -GRADIENT_SIZE)
				mScrollX = -GRADIENT_SIZE;
			break;
		case SCROLL_UP:
		case SCROLL_DOWN:
			mScrollY = mScrollY + mYDir*mSpeed;
			if(mYDir > 0 && mScrollY > GRADIENT_SIZE)
				mScrollY = GRADIENT_SIZE;
			else if(mYDir < 0 && mScrollY < -GRADIENT_SIZE)
				mScrollY = -GRADIENT_SIZE;
			break;
	}
}

void ScrollFadeEffect::Draw(float interp)
{
	if(!isActive)
		return;

	float interpScrollX = Lerp(interp, 0.0f, 1.0f, oldScrollX, mScrollX);
	float interpScrollY = Lerp(interp, 0.0f, 1.0f, oldScrollY, mScrollY);

	if(!mInOut)
	{
		switch(mKind)
		{
			case SCROLL_LEFT:
				lowDisplayFillRect(interpScrollX - GRADIENT_SIZE, 0, interpScrollX + mk::Core::getBaseWidth() - GRADIENT_SIZE, mk::Core::getBaseHeight(), 0x000000FF);
				lowDisplayGradientRect(interpScrollX + mk::Core::getBaseWidth() - GRADIENT_SIZE, 0, interpScrollX + mk::Core::getBaseWidth(), mk::Core::getBaseHeight(), 
					0x000000FF, 0x000000FF, 0x00000000, 0x00000000);
				break;
			case SCROLL_RIGHT:
				lowDisplayFillRect(interpScrollX + GRADIENT_SIZE, 0, interpScrollX + mk::Core::getBaseWidth() + GRADIENT_SIZE, mk::Core::getBaseHeight(), 0x000000FF);
				lowDisplayGradientRect(interpScrollX, 0, interpScrollX + GRADIENT_SIZE, mk::Core::getBaseHeight(),
					0x00000000, 0x00000000, 0x000000FF, 0x000000FF);
				break;
			case SCROLL_UP:
				lowDisplayFillRect(0, interpScrollY - GRADIENT_SIZE, mk::Core::getBaseWidth(), interpScrollY + mk::Core::getBaseHeight() - GRADIENT_SIZE, 0x000000FF);
				lowDisplayGradientRect(0, interpScrollY + mk::Core::getBaseHeight() - GRADIENT_SIZE, mk::Core::getBaseWidth(), interpScrollY + mk::Core::getBaseHeight(), 
					0x000000FF, 0x000000FF, 0x00000000, 0x00000000);
				break;
			case SCROLL_DOWN:
				lowDisplayFillRect(0, interpScrollY + GRADIENT_SIZE, mk::Core::getBaseWidth(), interpScrollY + mk::Core::getBaseHeight() + GRADIENT_SIZE, 0x000000FF);
				lowDisplayGradientRect(0, interpScrollY, mk::Core::getBaseWidth(), interpScrollY + GRADIENT_SIZE,
					0x00000000, 0x00000000, 0x000000FF, 0x000000FF);
				break;
		};
	}
	else
	{
		switch(mKind)
		{
			case SCROLL_LEFT:
				lowDisplayFillRect(interpScrollX + mk::Core::getBaseWidth(), 0, interpScrollX + mk::Core::getBaseWidth()*2, mk::Core::getBaseHeight(), 0x000000FF);
				lowDisplayGradientRect(interpScrollX + mk::Core::getBaseWidth() - GRADIENT_SIZE, 0, interpScrollX + mk::Core::getBaseWidth(), mk::Core::getBaseHeight(),
					0x00000000, 0x00000000, 0x000000FF, 0x000000FF);
				break;
			case SCROLL_RIGHT:
				lowDisplayFillRect(interpScrollX -mk::Core::getBaseWidth(), 0, interpScrollX, mk::Core::getBaseHeight(), 0x000000FF);
				lowDisplayGradientRect(interpScrollX, 0, interpScrollX + GRADIENT_SIZE, mk::Core::getBaseHeight(), 
					0x000000FF, 0x000000FF, 0x00000000, 0x00000000);
				break;
			case SCROLL_UP:
				lowDisplayFillRect(0, interpScrollY + mk::Core::getBaseHeight(), mk::Core::getBaseWidth(), interpScrollY + mk::Core::getBaseHeight()*2, 0x000000FF);
				lowDisplayGradientRect(0, interpScrollY + mk::Core::getBaseHeight() - GRADIENT_SIZE, mk::Core::getBaseWidth(), interpScrollY + mk::Core::getBaseHeight(),
					0x00000000, 0x00000000, 0x000000FF, 0x000000FF);
				break;
			case SCROLL_DOWN:
				lowDisplayFillRect(0, interpScrollY -mk::Core::getBaseHeight(), mk::Core::getBaseWidth(), interpScrollY, 0x000000FF);
				lowDisplayGradientRect(0, interpScrollX, mk::Core::getBaseWidth(), interpScrollY + GRADIENT_SIZE, 
					0x000000FF, 0x000000FF, 0x00000000, 0x00000000);
				break;
		};
	}
}

void ScrollFadeEffect::StartEffect()
{
	mTimer = 0.0f;
	isActive = true;
}

void ScrollFadeEffect::StopEffect()
{
	isActive = false;
}