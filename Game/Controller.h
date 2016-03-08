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
	DOWN,
	LB,
	RB
};

class Controller{
public:
	Controller();
	~Controller();

	void initialize(Manager* m);

	Vector movement();

	Vector camera();

	bool isPressed(const Command& c);

	bool usingController = false;
private:
	bool isPressed(const unsigned int& controllerId, const unsigned int& button);
	bool isFirstPressed(const unsigned int& controllerId, const unsigned int& button);
	float axisPosition(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	float axisPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);

	std::map<unsigned int, std::map<unsigned int, bool>> buttonStates;

	InputManager* im;
};