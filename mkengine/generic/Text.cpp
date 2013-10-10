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

	// ========================
	TextDynamicLetters::TextDynamicLetters(std::string _text, float _delay, mk::Font* _fnt, float _scale, u32 _color)
	{
		text = _text;
		scale = _scale;
		delay = _delay;
		color = _color;
		fnt = _fnt;

		lettersAlpha.resize(text.length() );
		for(int i = 0; i < lettersAlpha.size(); i++) {
			lettersAlpha[i] = 0.0f;
		}
		
		currentCounter = 0.0f;
		currentLetter = 0;
	}

	void TextDynamicLetters::Update()
	{
		currentCounter =+ 1.0f/30.0f;
		if(currentCounter >= delay)
		{
			currentCounter = 0.0f;
			currentLetter++;
		}

		if(currentLetter > text.length() - 1)
			currentLetter = text.length() - 1;
	}

	void TextDynamicLetters::Draw()
	{
		int mLength = text.length();
		float mXPos = posX;

		for(int i = 0; i < currentLetter; i++)
		{
			unsigned char c = text[i];
			sCharacter& mGfxChar = fnt->mChars[c];
			lowDisplayImg(fnt->mTex, mXPos + (float)mGfxChar.xoff*scale, posY + (float)mGfxChar.yoff*scale, (float)mGfxChar.x, (float)mGfxChar.y, 
				(float)mGfxChar.w, (float)mGfxChar.h, (float)mGfxChar.w/(float)fnt->mTex->getImageWidth()*scale, (float)mGfxChar.h/(float)fnt->mTex->getImageHeight()*scale, 0.0f, color);
			mXPos += (float)mGfxChar.xAdvance*scale;
		}
	}

};