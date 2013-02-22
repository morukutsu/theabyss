// project_abyss
// LittleSpace Studio 2012

//Includes
#include "DebugItems.h"
#include "DebugMenu.h"
#include "mkengine.h"

// -- DebugItemButton ----------------------------------
DebugItemButton::DebugItemButton(std::string _label, u32 tColor) 
{
	itemHeight = 32;
	label = _label;
	textColor = tColor;
}

void DebugItemButton::Draw()
{
	lowDisplayText(menu->getFont(), posX, posY, label.c_str(), textColor, 1.0f);
}

int DebugItemButton::getItemWidth()
{
	return mk::Text::GetStringWidth(menu->getFont(), label.c_str(), 1.0f);
}
