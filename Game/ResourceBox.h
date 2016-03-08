#pragma once

#include "Menu.h"
#include "Manager.h"
#include "Inventory.h"
#include "Animatable.h"
#include "Controller.h"
#include "PlayerInventory.h"

class ResourceBox: public Inventory, public InputListener, public drawable::Drawable{
public:
	ResourceBox(Controller* controller, Manager* manager, const unsigned char& size, PlayerInventory* pi, drawable::Drawable* box);
	~ResourceBox();

	ItemStack& selectedItem();

	void update();

	void on(MouseButtonEvent& event);

	Manager* manager;

	PlayerInventory* pi;

	Menu* menu;
	int selectedSlot;
	CoreSprite* handle;
	Controller* controller;
private:
	void updateMenu();

	unsigned long listenerId;
	std::string id;
	std::string* pressed;
};