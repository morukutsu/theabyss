// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _DEBUGITEMS_H
#define _DEBUGITEMS_H

#include <string>
#include "mkengine.h"

// -- DebugItem ----------------------------------
class DebugMenu;
class DebugItem {
	public:
		DebugItem() {};

		virtual void Draw() = 0;
		virtual int getItemWidth() = 0;

		void MoveTo(float x, float y) { posX = x, posY = y; };
		int getItemHeight() { return itemHeight; };
		void SetMenu(DebugMenu * m) { menu = m; };
		void SetItemId(int id) { itemId = id; };
		int GetItemId() { return itemId; };

	protected:
		int itemHeight, itemId;
		float posX, posY;
		DebugMenu* menu;
};

// -- DebugItemButton ----------------------------------
class DebugItemButton : public DebugItem {
	public:
		DebugItemButton(std::string label, u32 textColor = 0x000000FF);
		void Draw();
		int getItemWidth();
		std::string GetLabel() { return label; };

	private:
		std::string label;
		u32 textColor;
};

#endif