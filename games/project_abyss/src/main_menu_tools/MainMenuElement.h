// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MAINMENUELEMENT_H
#define _MAINMENUELEMENT_H

#include <string>
#include "mkengine.h"

class MainMenu;

class MainMenuElement {
	public:
		MainMenuElement() {};

		virtual void Draw(float interp) = 0;
		virtual void Update() = 0;
		virtual void Reset() = 0;
		virtual int getItemWidth() = 0;

		void MoveTo(float x, float y) { posX = x, posY = y; };
		int getItemHeight() { return itemHeight; };
		void SetMenu(MainMenu * m) { menu = m; };
		void SetItemId(int id) { itemId = id; };
		int GetItemId() { return itemId; };
		int getItemMarginBottom() { return itemMarginBottom; };

	protected:
		int itemHeight, itemId;
		int itemMarginBottom;

		float posX, posY;
		MainMenu* menu;
	
	public:
		bool selected, oldSelected;
};

#endif