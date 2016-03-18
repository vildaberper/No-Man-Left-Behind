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
	RB,
	LT,
	RT,
	MENUUP,
	MENUDOWN,
	HANDBOOK,
	BACK,
	SKIP
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

	float axisPressedValue = 0.9f;
	float axisThreshold = 0.25f;
private:
	bool isPressed(const unsigned int& controllerId, const unsigned int& button);
	bool isFirstPressed(const unsigned int& controllerId, const unsigned int& button);
	bool isAxisFirstPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	bool isTriggerNegAxisFirstPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	bool isTriggerPosAxisFirstPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	float axisPosition(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	float axisNegPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	float axisPosPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);
	float axisPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis);

	std::map<unsigned int, std::map<unsigned int, bool>> buttonStates;

	std::map<unsigned int, std::map<sf::Joystick::Axis, bool>> axisStates;

	std::map<unsigned int, std::map<sf::Joystick::Axis, bool>> axisNegStates;
	std::map<unsigned int, std::map<sf::Joystick::Axis, bool>> axisPosStates;

	InputManager* im;
};