// project_abyss
// LittleSpace Studio 2012

//Includes
#include "MainMenuTextElement.h"
#include "MainMenu.h"
#include "mkengine.h"
#include "../pennereasing/Back.h"

// -- DebugItemButton ----------------------------------
MainMenuTextElement::MainMenuTextElement(std::string _label, float scale, int fheight, u32 tColor, u32 sColor) 
{
	itemHeight = fheight;
	label = _label;
	textColor = tColor;
	shadowColor = sColor;
	fontScale = scale; 
	animate = false;
	currentTime = 0.0f;
}

void MainMenuTextElement::Update()
{
	if(this->menu->isVisible)
		currentTime += 1.0f/30.0f;
}

void MainMenuTextElement::Reset()
{
	currentTime = 0.0f;
}

void MainMenuTextElement::Draw(float interp)
{
	float posXInterp = posX;
	if(animate)
	{
		float interpTime = currentTime - offsetTime + 1.0f/30.0f*interp;
		interpTime = clampf(interpTime, 0.0f, 1.0f);

		posXInterp = Back::easeOut(interpTime, posX + offsetStartPos, -offsetStartPos, 1.0f);
	}

	mk::Text::DrawShadowedText(menu->getFont(), posXInterp, posY, label.c_str(), 
		textColor, shadowColor, 4.0f, fontScale);		
}

int MainMenuTextElement::getItemWidth()
{
	return mk::Text::GetStringWidth(menu->getFont(), label.c_str(), fontScale);
}

void MainMenuTextElement::Animate(float _offsetTime, float _offsetStartPos)
{
	animate = true;
	offsetTime = _offsetTime;
	offsetStartPos = _offsetStartPos;
}
