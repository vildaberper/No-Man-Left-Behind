#pragma once

#include "Manager.h"
#include "Drawable.h"

class Player : public drawable::Drawable{
public:
	Player();
	~Player();

	void initialize(Manager* m);
};