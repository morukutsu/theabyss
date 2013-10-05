// project_abyss
// LittleSpace Studio 2012

//Includes
#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "mkengine.h"
#include <string>
#include <list>
#include "MainMenuElement.h"

class MainMenu {
	public:
		MainMenu(mk::Font* font);
		~MainMenu();

		void MoveTo(float x, float y) { posX = x, posY = y; };
		void ToggleFocus(bool focus) { isFocus = focus; };
		void UpdateAndDraw(float interp);
		void Update();
		mk::Font* getFont() { return font; };
		void Show();
		void Hide();

		void AddItem(MainMenuElement* item);
		int GetSelectedItemId();
		int GetPressedItemId();
		void ClearMenu();
		int GetItemCount() { return items.size(); }
		MainMenuElement* GetSelectedItem() { return selectedItem; };

	public:
		float posX, posY;
		bool oldVisible, isVisible, isFocus;
		std::list<MainMenuElement*> items;
		mk::Font* font;
		MainMenuElement* selectedItem;
		int currentItemId;
};

#endif
