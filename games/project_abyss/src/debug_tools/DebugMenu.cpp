// project_abyss
// LittleSpace Studio 2012

#include "DebugMenu.h"
#include "DebugItems.h"

DebugMenu::DebugMenu(mk::Font* fnt)
{
	items.clear();
	isVisible = true;
	isFocus = true;
	posX = posY = 0;
	font = fnt;
	selectedItem = NULL;
	currentItemId = 0;
}

DebugMenu::~DebugMenu()
{
	for(std::list<DebugItem*>::iterator it = items.begin(); it != items.end(); )
	{
		delete (*it);
		items.erase(it++);
	}
}

void DebugMenu::UpdateAndDraw() 
{
	// Input
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	// Selection par defaut
	selectedItem = NULL;

	// Selection à l'aide du pointeur de souris
	float currentItemPositionY = posY;
	if(isFocus) {
		for(std::list<DebugItem*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			float a = posX - 2;
			float b = currentItemPositionY - 2;
			float c = posX + (*it)->getItemWidth() + 2;
			float d = currentItemPositionY + (*it)->getItemHeight() + 2;

			if(input->pointer.x >= a && input->pointer.x <= c && input->pointer.y >= b && input->pointer.y <= d)
			{
				selectedItem = (*it);
				break;
			}
		
			currentItemPositionY += (*it)->getItemHeight(); 
		}
	}

	// Affichage
	if(isVisible) 
	{
		currentItemPositionY = posY;
		for(std::list<DebugItem*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			// Affichage du selecteur
			if((*it) == selectedItem) {
				lowDisplayFillRect(posX - 2, currentItemPositionY - 2, 
					posX + (*it)->getItemWidth() + 2, currentItemPositionY + (*it)->getItemHeight() + 2, 
					MK_MAKE_RGBA(0, 0, 255, 64));
			}

			// Affichage de l'item
			(*it)->MoveTo(posX, currentItemPositionY);
			(*it)->Draw();

			currentItemPositionY += (*it)->getItemHeight(); 
		}
	}
}

void DebugMenu::AddItem(DebugItem* item) {
	item->SetMenu(this);
	item->SetItemId(currentItemId);

	items.push_back(item);
	currentItemId++;
}

int DebugMenu::GetSelectedItemId()
{
	if(selectedItem == NULL)
		return -1;

	return selectedItem->GetItemId();
}

int DebugMenu::GetPressedItemId()
{
	if(selectedItem == NULL)
		return -1;

	// Input
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	if(input->buttons[mk::ButtonNames::LeftClick].pressed)
		return selectedItem->GetItemId();

	return -1;
}

void DebugMenu::ClearMenu()
{
	items.clear();
	selectedItem = NULL;
	currentItemId = 0;
}