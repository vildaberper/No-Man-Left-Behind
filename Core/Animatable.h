#pragma once

#include "Manager.h"
#include "Drawable.h"

class Animatable : public drawable::Drawable{
public:
	Animatable();
	~Animatable();

	void apply(Manager* m, const std::string& a);

	virtual void tick(const sf::Time& time, const float& dt);
};