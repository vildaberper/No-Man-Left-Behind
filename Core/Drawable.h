#pragma once

#include "Entity.h"
#include "SFMLI.h"

class Drawable : public Entity{
public:
	Drawable();
	~Drawable();

	sf::Sprite* getSprite(const sf::Time& time);
};