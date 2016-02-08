#pragma once

#include "Manager.h"
#include "Animatable.h"

class Player : public Animatable{
public:
	Player();
	~Player();

	void initialize(Manager* m);
};