#include "PlayerInventory.h"

namespace container{
	long id = 0;
}

PlayerInventory::PlayerInventory(Controller* controller, Manager* manager, const unsigned char& size){
	id = "container" + std::to_string(container::id++);

	pressed = new std::string();
	menuInHand = new MenuItem();
	menuInHand->closeOnClick = false;
	menuInHand->alternativeText = true;
	menuInHand->type = TEXTURE;

	PlayerInventory::manager = manager;
	PlayerInventory::controller = controller;
	setSize(size);

	menu = new Menu();
	menu->hidden = false;
	menu->type = HORIZONTAL;
	menu->background = new TexBar(
		manager->spriteManager->getTexture("bag.left"),
		manager->spriteManager->getTexture("bag.middle"),
		manager->spriteManager->getTexture("bag.right")
		);
	menu->drawElementBackgrounds = false;
	menu->leftOffset = menu->rightOffset = 20.0f;
	menu->topOffset = 30.0f;
	manager->menuManager->menus[id] = menu;

	handle = manager->spriteManager->getSprite("bag.handle");

	listenerId = manager->inputManager->registerListener(this);

	menu->hidden = true;

	update();
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
	handle->sprite()->setPosition(
		(gi::TARGET_WIDTH / 2.0f - handle->w() / 2.0f) * gi::dxiz(),
		(gi::TARGET_HEIGHT - 115.0f - handle->h() / 2.0f) * gi::dyiz()
		);
	handle->sprite()->scale(
		(1.0f / handle->sprite()->getScale().x) * 1.0f * gi::dxiz(),
		(1.0f / handle->sprite()->getScale().y) * 1.0f * gi::dyiz()
		);
	gi::draw(*handle->sprite());
	if(itemInHand.amount > 0){
		float w = 80.0f * gi::dxiz();
		float h = 100.0f * gi::dyiz();
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
		if(event.button() == sf::Mouse::Left){
			size_t index = 0;
			if((index = pressed->find_first_of(':')) != std::string::npos && pressed->substr(0, index) == id){
				unsigned char c = unsigned char(stoi(pressed->substr(index + 1, pressed->find_first_of(';'))));
				if(itemInHand.amount > 0){
					si::playRandomSoundV(nullptr, "bag", gc::bagVolume);
				}
				if(itemInHand.amount > 0 && itemInHand.item.type == at(c).item.type){
					put(itemInHand, selectedSlot = c);
				}
				else{
					swap(itemInHand, selectedSlot = c);
				}
				update();
				*pressed = "";
			}
		}
	}
	if(event.pressed() && event.button() == sf::Mouse::Right){
		event.setCancelled(true);
		put(itemInHand, selectedSlot);
		if(itemInHand.amount > 0){
			put(itemInHand);
		}
		update();
	}
}

void PlayerInventory::updateMenu(){
	float w = getSize() * 80.0f + 40;
	menu->position = Vector(gi::TARGET_WIDTH / 2 - w / 2.0f, gi::TARGET_HEIGHT - 120.0f);
	menu->size = Vector(w, 120.0f);

	selectedSlot = math::range(selectedSlot, getSize() - 1);

	while(menu->items.size() > getSize()){
		menu->items.pop_back();
	}
	while(menu->items.size() < getSize()){
		MenuItem* ti = new MenuItem();
		ti->closeOnClick = false;
		ti->alternativeText = true;
		ti->selectedString = pressed;
		ti->selectedPrefix = id + ":" + std::to_string(menu->items.size()) + ";";
		menu->items.push_back(ti);
	}
	for(unsigned char i = 0; i < getSize(); i++){
		ItemStack is = at(i);
		MenuItem* ti = menu->items[i];
		if(is.amount > 0){
			ti->type = TEXTURE;
			if(is.amount == 1){
				ti->title = "";
			}
			else{
				ti->title = std::to_string(is.amount);
			}
			ti->sprite = manager->spriteManager->getSprite("Resources." + resourceToString(is.item.type));
		}
		else{
			ti->title = "";
			ti->type = TEXT;
		}
		ti->highlight = controller->usingController && i == selectedSlot;
	}

	if(itemInHand.amount == 1){
		menuInHand->title = "";
	}
	else{
		menuInHand->title = std::to_string(itemInHand.amount);
	}
	menuInHand->sprite = manager->spriteManager->getSprite("Resources." + resourceToString(itemInHand.item.type));
}