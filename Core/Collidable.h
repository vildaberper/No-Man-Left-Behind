#pragma once

#include "Drawable.h"

class Collidable: public drawable::Drawable{
public:
	Collidable();
	~Collidable();

	sf::FloatRect bounds(const sf::Time& time);

	bool collidesWith(Collidable* c, const sf::Time& time);

	Vector offset;
	Vector size;
};