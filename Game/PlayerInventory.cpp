#include "PlayerInventory.h"

namespace container{
	long id = 0;
}

PlayerInventory::PlayerInventory(Manager* manager, const unsigned char& size){
	id = "container" + std::to_string(container::id++);

	pressed = new std::string();
	menuInHand = new MenuItem();
	menuInHand->closeOnClick = false;
	menuInHand->alternativeText = true;
	menuInHand->type = TEXTURE;

	PlayerInventory::manager = manager;
	setSize(size);

	menu = new Menu();
	menu->hidden = false;
	menu->type = HORIZONTAL;
	menu->position = Vector(gi::TARGET_WIDTH / 2 - 350, gi::TARGET_HEIGHT - 100.0f);
	menu->size = Vector(700, 100);
	for(unsigned char i = 0; i < getSize(); i++){
		MenuItem* ti = new MenuItem();
		ti->closeOnClick = false;
		ti->alternativeText = true;
		ti->selectedString = pressed;
		ti->selectedPrefix = std::to_string(i) + ":";
		menu->items.push_back(ti);
	}
	menu->background = new TexBar(
		manager->spriteManager->getTexture("bag.left"),
		manager->spriteManager->getTexture("bag.middle"),
		manager->spriteManager->getTexture("bag.right")
		);
	menu->drawElementBackgrounds = false;
	menu->leftOffset = menu->rightOffset = 10.0f;
	menu->topOffset = 20.0f;
	manager->menuManager->menus[id] = menu;

	listenerId = manager->inputManager->registerListener(this);

	menu->hidden = true;
}

PlayerInventory::~PlayerInventory(){
	manager->inputManager->unregisterListener(listenerId);
	manager->menuManager->menus.erase(id);
	delete menu;
	delete pressed;
	delete menuInHand;
}

ItemStack& PlayerInventory::selectedItem(){
	return at(selectedSlot);
}

void PlayerInventory::update(){
	updateMenu();
}

void PlayerInventory::render(){
	if(itemInHand.amount > 0){
		float w = 80.0f * gi::dxiz();
		float h = 80.0f * gi::dyiz();
		gi::draw(
			menuInHand,
			sf::milliseconds(0),
			float(manager->inputManager->mouseX()) - w / 2.0f,
			float(manager->inputManager->mouseY()) - h / 2.0f,
			w,
			h,
			false
			);
	}
}

void PlayerInventory::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		swap(itemInHand, unsigned char(stoi(pressed->substr(0, pressed->find_first_of(':')))));
		updateMenu();
	}
}

void PlayerInventory::updateMenu(){
	selectedSlot = math::range(selectedSlot, getSize() - 1);
	for(unsigned char i = 0; i < getSize(); i++){
		ItemStack is = at(i);
		MenuItem* ti = menu->items[i];
		if(is.amount > 0){
			ti->type = TEXTURE;
			ti->title = std::to_string(is.amount);
			ti->sprite = manager->spriteManager->getSprite("Resources." + resourceToString(is.item.type));
		}
		else{
			ti->title = "";
			ti->type = TEXT;
		}
		ti->highlight = i == selectedSlot;
	}

	menuInHand->title = std::to_string(itemInHand.amount);
	menuInHand->sprite = manager->spriteManager->getSprite("Resources." + resourceToString(itemInHand.item.type));
}