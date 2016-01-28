#include "Drawable.h"

Drawable::Drawable(){

}


Drawable::~Drawable(){

}

sf::Sprite* Drawable::getSprite(const sf::Time& time){
	return new sf::Sprite;	// PLACEHOLDER
}