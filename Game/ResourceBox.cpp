#include "ResourceBox.h"

ResourceBox::ResourceBox(Controller* controller, Manager* manager, const unsigned char& size, PlayerInventory* pi, drawable::Drawable* box){
	for(auto &ent : box->animations){
		animations[ent.first] = ent.second->clone();
	}
	currentAnimation = box->currentAnimation;
	nextAnimation = box->nextAnimation;
	cb = box->cb;
	position = box->position;
	reference = box->reference;
	scale = box->scale;
	startTime = box->startTime;
	box->kill();

	id = "container" + std::to_string(container::id++);

	ResourceBox::pi = pi;

	pressed = new std::string();

	ResourceBox::manager = manager;
	ResourceBox::controller = controller;
	setSize(size);

	menu = new Menu();
	menu->hidden = true;
	menu->type = HORIZONTAL;
	menu->background = new TexBar(
		/*manager->spriteManager->getTexture("box.left"),
		manager->spriteManager->getTexture("box.middle"),
		manager->spriteManager->getTexture("box.right")*/
		NULL,
		manager->spriteManager->getTexture("box.full"),
		NULL
		);
	menu->drawElementBackgrounds = false;
	menu->topOffset = 5.0f;
	menu->bottomOffset = 70.0f;
	manager->menuManager->menus[id] = menu;

	handle = manager->spriteManager->getSprite("bag.handle");

	listenerId = manager->inputManager->registerListener(this);

	menu->hidden = true;

	update();
}

ResourceBox::~ResourceBox(){
	manager->inputManager->unregisterListener(listenerId);
	manager->menuManager->menus.erase(id);
	delete menu;
	delete pressed;
}

ItemStack& ResourceBox::selectedItem(){
	return at(selectedSlot);
}

void ResourceBox::update(){
	updateMenu();
}

void ResourceBox::on(MouseButtonEvent& event){
	if(menu->hidden)
		return;
	if(event.isCancelled()){
		if(event.button() == sf::Mouse::Left){
			size_t index = 0;
			if((index = pressed->find_first_of(':')) != std::string::npos && pressed->substr(0, index) == id){
				unsigned char c = unsigned char(stoi(pressed->substr(index + 1, pressed->find_first_of(';'))));
				if(pi->itemInHand.amount > 0 && pi->itemInHand.item.type == at(c).item.type){
					put(pi->itemInHand, selectedSlot = c);
				}
				else{
					swap(pi->itemInHand, selectedSlot = c);
				}
				pi->update();
				update();
				*pressed = "";
			}
		}
	}
}

void ResourceBox::updateMenu(){
	float w = getSize() * 128.0f;
	menu->position = Vector(gi::TARGET_WIDTH / 2 - w / 2.0f, gi::TARGET_HEIGHT / 2);
	menu->size = Vector(w, 192.0f);

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
}