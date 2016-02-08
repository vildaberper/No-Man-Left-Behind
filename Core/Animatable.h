#pragma once

#include "Drawable.h"

class Animatable : public drawable::Drawable{
public:
	Animatable();
	~Animatable();

	virtual void tick(const sf::Time& time, const float& dt);
};