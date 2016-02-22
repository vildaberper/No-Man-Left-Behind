#pragma once

#include "Manager.h"
#include "Inventory.h"
#include "Animatable.h"

class Player : public Animatable{
public:
	Player();
	~Player();

	void initialize(Manager* m);

	virtual void tick(const sf::Time& time, const float& dt);

	float speed;

	Inventory* inventory;
};