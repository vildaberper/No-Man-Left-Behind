#pragma once

#include "SFMLI.h"
#include "Entity.h"

class Drawable : public Entity{
public:
	Drawable();
	~Drawable();

	sf::Sprite* getSprite();
};