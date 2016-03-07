#pragma once

#include "Menu.h"
#include "Manager.h"
#include "Inventory.h"

namespace container{
	extern long id;
}

class PlayerInventory: public Inventory, InputListener{
public:
	PlayerInventory(Manager* manager, const unsigned char& size);
	~PlayerInventory();

	ItemStack& selectedItem();

	void update();

	void render();

	void on(MouseButtonEvent& event);

	Manager* manager;

	Menu* menu;
	int selectedSlot;
	ItemStack itemInHand;
	MenuItem* menuInHand;
	CoreSprite* handle;
private:
	void updateMenu();

	unsigned long listenerId;
	std::string id;
	std::string* pressed;
};