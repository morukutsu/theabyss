// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MAINMENUTEXTELEMENT_H
#define _MAINMENUTEXTELEMENT_H

#include "MainMenuElement.h"

class MainMenuTextElement : public MainMenuElement {
	public:
		MainMenuTextElement(std::string label, std::string hint, float scale, int height, int marginBot, u32 textColor = 0x000000FF, u32 sColor = 0x00000000);
		void Draw(float interp);
		void Update();
		void Reset();

		int getItemWidth();
		std::string GetLabel() { return label; };
		std::string GetHint() { return hint; };
		void Animate(float offsetTime, float offsetStartPos);

	private:
		std::string label, hint;
		u32 textColor, shadowColor;
		float fontScale;
		bool animate;
		float offsetTime, offsetStartPos, currentTime;
		float selectTime;
};


#endif