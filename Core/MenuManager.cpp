#include "MenuManager.h"

using namespace sf;

MenuManager::MenuManager(InputManager* inputManager){
	inputManager_ = inputManager;
	inputManager_->registerListener(this);
}

MenuManager::~MenuManager(){

}

void MenuManager::initialize(RenderWindow* window){

}

void MenuManager::finalize(RenderWindow* window){

}

void MenuManager::tick(RenderWindow* window, const Time& time, const float& dt){

}

void MenuManager::draw(const sf::Time& time){
	for (auto &ent : menus){
		if (!ent.second->hidden){
			gi::draw(ent.second, time);
		}
	}
}

void MenuManager::on(MouseButtonEvent& event){
	if (event.pressed() && event.button() == sf::Mouse::Button::Left){
		for (auto &ent : menus){
			if (!ent.second->hidden){
				float x = ent.second->position.x * gi::dxiz();
				float y = ent.second->position.y * gi::dyiz();
				float w = ent.second->size.x * gi::dxiz();
				float h = ent.second->size.y * gi::dyiz();
				if (event.x() >= x && event.x() <= x + w){
					if (event.y() >= y && event.y() <= y + h){
						event.setCancelled(true);

						size_t index;
						MenuItem* mi = NULL;
						switch (ent.second->type){
						case HORIZONTAL:
						{
										   index = size_t(floor(ent.second->items.size() * ((event.x() - x) / w)));
										   mi = ent.second->items[index];
										   break;
						}
						case VERTICAL:
						{
										 index = size_t(floor(ent.second->items.size() * ((event.y() - y) / h)));
										 mi = ent.second->items[index];
										 break;
						}
						}
						if(mi->toggle != NULL){
							mi->toggle->hidden = !mi->toggle->hidden;
						}
						if (mi->closeOnClick){
							ent.second->hidden = true;
						}
						if (mi->selectedString != NULL){
							*mi->selectedString = mi->selectedPrefix + mi->title;
						}
					}
				}
			}
		}
	}
}
