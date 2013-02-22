// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_FONT
#define MK_FONT

#include "Image.h"
#include "Ressource.h"

struct sCharacter
{
	unsigned char id;
	int x, y;
	short xoff, yoff, w, h;
	short xAdvance;
};

// Font : bitmap font loader

namespace mk 
{
	class Font : public Ressource
	{
		public:
			Font();
			~Font();

			int LoadFromFile(const char* filename);		// Loads a Font image from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The Font pointer must be positionned on the Font
			int LoadFromMemory(char* mem, int size);	// Load a Font from memory

		public:
			char mName[256];
			sCharacter mChars[256];
			Image* mTex;
			int mLineHeight;
			int mCharNbr;
	};
}
#endif