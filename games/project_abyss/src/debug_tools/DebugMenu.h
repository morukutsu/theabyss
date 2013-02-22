// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _DEBUGMENU_H
#define _DEBUGMENU_H

#include "mkengine.h"
#include <string>
#include <list>
#include "DebugItems.h"

class DebugMenu {
	public:
		DebugMenu(mk::Font* font);
		~DebugMenu();

		void MoveTo(float x, float y) { posX = x, posY = y; };
		void Show() { isVisible = true; };
		void Hide() { isVisible = false; };
		void ToggleFocus(bool focus) { isFocus = focus; };
		void UpdateAndDraw();
		mk::Font* getFont() { return font; };

		void AddItem(DebugItem* item);
		int GetSelectedItemId();
		int GetPressedItemId();
		void ClearMenu();
		int GetItemCount() { return items.size(); }
		DebugItem* GetSelectedItem() { return selectedItem; };

	private:
		float posX, posY;
		bool isVisible, isFocus;
		std::list<DebugItem*> items;
		mk::Font* font;
		DebugItem* selectedItem;
		int currentItemId;
};

#endif