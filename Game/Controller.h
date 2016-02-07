#pragma once

#include "Manager.h"
#include "Vector.h"
#include "SFMLI.h"

enum Command{
	INTERACT,
	PAUSE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Controller{
public:
	Controller();
	~Controller();

	void initialize(Manager* m);

	Vector movement();

	bool isPressed(const Command& c);
private:
	InputManager* im;
};