// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "Text.h"

/*void DrawString(Font* font, int x, int y, char* text, sColor color, float scale)
{
	int mLength = strlen(text);
	float mXPos = x;
	font->mTex.Alpha((float)color.a/255);
	for(int i = 0; i < mLength; i++)
	{
		unsigned char c = text[i];
		sCharacter mGfxChar = font->mChars[c];
		font->mTex.Clip(mGfxChar.x, mGfxChar.y, mGfxChar.w, mGfxChar.h);
		font->mTex.SetSize((float)mGfxChar.w*scale, (float)mGfxChar.h*scale);
		font->mTex.MoveTo(mXPos + (float)mGfxChar.xoff*scale, y + (float)mGfxChar.yoff*scale);
		font->mTex.Tint(color.r, color.g, color.b);
		font->mTex.Draw();
		mXPos += (float)mGfxChar.xAdvance*scale;
	}
}

int GetStringWidth(Font* font, char* text, float scale)
{
	int mLength = strlen(text);
	float mXPos = 0;
	for(int i = 0; i < mLength; i++)
	{
		unsigned char c = text[i];
		sCharacter mGfxChar = font->mChars[c];
		mXPos += (float)mGfxChar.xAdvance*scale;
	}

	return (int)mXPos;
}

void DrawStringBox(Font* font, int x, int y, int w, int h, char* text, sColor color, float scale)
{
	

}*/

namespace mk
{
	float Text::GetStringWidth(mk::Font* font, const char* text, float scale)
	{
		int mLength = strlen(text);
		float mXPos = 0;
		for(int i = 0; i < mLength; i++)
		{
			unsigned char c = text[i];
			sCharacter& mGfxChar = font->mChars[c];
			mXPos += (float)mGfxChar.xAdvance*scale;
		}

		return mXPos;
	}

	void Text::DrawShadowedText(mk::Font* fnt, float x, float y, const char* text, u32 color, u32 shadow_color, float shadow_distance, float scale)
	{
		// Shadow
		lowDisplayText(fnt, x + shadow_distance, y + shadow_distance, text, shadow_color, scale);

		// Text
		lowDisplayText(fnt, x, y, text, color, scale);
	}
};