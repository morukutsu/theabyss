// project_abyss
// LittleSpace Studio 2012

#include "MainMenu.h"
#include "MainMenuElement.h"

MainMenu::MainMenu(mk::Font* fnt)
{
	items.clear();
	isVisible = true;
	isFocus = true;
	posX = posY = 0;
	font = fnt;
	selectedItem = NULL;
	currentItemId = 0;
}

MainMenu::~MainMenu()
{
	for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); )
	{
		delete (*it);
		items.erase(it++);
	}
}

void MainMenu::UpdateAndDraw(float interp) 
{
	// Input
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	// Selection par defaut
	selectedItem = NULL;
	for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
		(*it)->selected = false;

	// Selection à l'aide du pointeur de souris
	float currentItemPositionY = posY;
	if(isFocus) {
		for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			float a = posX - 2;
			float b = currentItemPositionY - 2;
			float c = posX + (*it)->getItemWidth() + 2;
			float d = currentItemPositionY + (*it)->getItemHeight() + 2;

			if(input->pointer.x >= a && input->pointer.x <= c && input->pointer.y >= b && input->pointer.y <= d)
			{
				selectedItem = (*it);
				(*it)->selected = true;
				break;
			}
		
			currentItemPositionY += (*it)->getItemHeight() + (*it)->getItemMarginBottom(); 
		}
	}

	// Affichage
	if(isVisible) 
	{
		currentItemPositionY = posY;
		for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			// Affichage de l'item
			(*it)->MoveTo(posX, currentItemPositionY);
			(*it)->Draw(interp);

			currentItemPositionY += (*it)->getItemHeight() + (*it)->getItemMarginBottom(); 
		}
	}
}

void MainMenu::Update()
{
	oldVisible = isVisible;
	for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
	{
		(*it)->Update();
	}
}

void MainMenu::AddItem(MainMenuElement* item) {
	item->SetMenu(this);
	item->SetItemId(currentItemId);

	items.push_back(item);
	currentItemId++;
}

int MainMenu::GetSelectedItemId()
{
	if(selectedItem == NULL)
		return -1;

	return selectedItem->GetItemId();
}

int MainMenu::GetPressedItemId()
{
	if(selectedItem == NULL)
		return -1;

	// Input
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	if(input->buttons[mk::ButtonNames::LeftClick].pressed)
		return selectedItem->GetItemId();

	return -1;
}

void MainMenu::ClearMenu()
{
	items.clear();
	selectedItem = NULL;
	currentItemId = 0;
}

void MainMenu::Show() 
{ 
	isVisible = true;
	if(oldVisible != isVisible)
	{
		for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			(*it)->Reset();
		}
	}
}

void MainMenu::Hide() 
{ 
	isVisible = false;
	if(oldVisible != isVisible)
	{
		for(std::list<MainMenuElement*>::iterator it = items.begin(); it != items.end(); it++) 
		{
			(*it)->Reset();
		}
	}
}