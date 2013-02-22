// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_TEXT
#define MK_TEXT

#include "lowSystem.h"
#include "lowDisplay.h"
#include "Font.h"

//Fonctions génériques d'affichage de texte
//void DrawString(Font* font, int x, int y, char* text, sColor color, float scale = 1.0f);
//void DrawStringBox(Font* font, int x, int y, int w, int h, char* text, sColor color, float scale = 1.0f);

//Classe spécialisée
namespace mk
{
	class Text
	{
		public:
			static float GetStringWidth(mk::Font* font, const char* text, float scale);

		public:

	};
};

#endif