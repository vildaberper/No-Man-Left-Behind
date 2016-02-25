#include "Controller.h"

Controller::Controller(){

}

Controller::~Controller(){

}

void Controller::initialize(Manager* m){
	im = m->inputManager;
}

Vector Controller::movement(){
	Vector joystick = Vector(axisPosition(0, sf::Joystick::X), axisPosition(0, sf::Joystick::Y));
	if(joystick.length() > 0.0f){
		return joystick;
	}
	return Vector(
		(isPressed(LEFT) ? -1.0f : 0.0f) + (isPressed(RIGHT) ? 1.0f : 0.0f),
		(isPressed(UP) ? -1.0f : 0.0f) + (isPressed(DOWN) ? 1.0f : 0.0f)
		).norm();
}

Vector Controller::camera(){
	Vector joystick = Vector(axisPosition(0, sf::Joystick::U), axisPosition(0, sf::Joystick::R));
	if(joystick.length() > 0.0f){
		return joystick;
	}
	return Vector(0.0f, 0.0f);
}

bool Controller::isPressed(const Command& c){
	switch(c){
	case INTERACT:
		return isFirstPressed(0, 0) || im->isFirstPressed(sf::Keyboard::Key::E);
		break;
	case PAUSE:
		return isFirstPressed(0, 7) || im->isFirstPressed(sf::Keyboard::Key::P);
		break;
	case LEFT:
		return im->isPressed(sf::Keyboard::Key::A);
		break;
	case RIGHT:
		return im->isPressed(sf::Keyboard::Key::D);
		break;
	case UP:
		return im->isPressed(sf::Keyboard::Key::W);
		break;
	case DOWN:
		return im->isPressed(sf::Keyboard::Key::S);
		break;
	case RB:
		return isFirstPressed(0, 5);
		break;
	case LB:
		return isFirstPressed(0, 4);
		break;
	}
	return false;
}

bool Controller::isPressed(const unsigned int& controllerId, const unsigned int& button){
	if(sf::Joystick::isConnected(controllerId)){
		if(sf::Joystick::getButtonCount(controllerId) > button){
			return sf::Joystick::isButtonPressed(controllerId, button);
		}
	}
	return false;
}

bool Controller::isFirstPressed(const unsigned int& controllerId, const unsigned int& button){
	if(sf::Joystick::isConnected(controllerId)){
		if(sf::Joystick::getButtonCount(controllerId) > button){
			bool state = buttonStates[controllerId][button];

			return (buttonStates[controllerId][button] = sf::Joystick::isButtonPressed(controllerId, button)) && !state;
		}
	}
	return false;
}

float Controller::axisPosition(const unsigned int& controllerId, const sf::Joystick::Axis& axis){
	if(sf::Joystick::isConnected(controllerId)){
		if(sf::Joystick::hasAxis(controllerId, axis)){
			float value = sf::Joystick::getAxisPosition(controllerId, axis) / 100.0f;

			if(abs(value) > 0.25f){
				return value;
			}
		}
	}
	return 0.0f;
}

float Controller::axisPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis){
	return axisPosition(controllerId, axis) > 0.9f;
}