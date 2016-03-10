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
		if(im->isFirstPressed(sf::Keyboard::Key::E)){
			usingController = false;
			return true;
		}
		return isFirstPressed(0, 0);
		break;
	case PAUSE:
		if(im->isFirstPressed(sf::Keyboard::Key::P)){
			usingController = false;
			return true;
		}
		return isFirstPressed(0, 7);
		break;
	case LEFT:
		if(im->isPressed(sf::Keyboard::Key::A)){
			usingController = false;
			return true;
		}
		break;
	case RIGHT:
		if(im->isPressed(sf::Keyboard::Key::D)){
			usingController = false;
			return true;
		}
		break;
	case UP:
		if(im->isPressed(sf::Keyboard::Key::W)){
			usingController = false;
			return true;
		}
		break;
	case DOWN:
		if(im->isPressed(sf::Keyboard::Key::S)){
			usingController = false;
			return true;
		}
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
			if(sf::Joystick::isButtonPressed(controllerId, button)){
				usingController = true;
				return true;
			}
		}
	}
	return false;
}

bool Controller::isFirstPressed(const unsigned int& controllerId, const unsigned int& button){
	if(sf::Joystick::isConnected(controllerId)){
		if(sf::Joystick::getButtonCount(controllerId) > button){
			bool state = buttonStates[controllerId][button];

			if(sf::Joystick::isButtonPressed(controllerId, button)){
				buttonStates[controllerId][button] = true;
				usingController = true;
				if(!state){
					return true;
				}
			}
			else{
				buttonStates[controllerId][button] = false;
			}
		}
	}
	return false;
}

float Controller::axisPosition(const unsigned int& controllerId, const sf::Joystick::Axis& axis){
	if(sf::Joystick::isConnected(controllerId)){
		if(sf::Joystick::hasAxis(controllerId, axis)){
			float value = sf::Joystick::getAxisPosition(controllerId, axis) / 100.0f;

			if(abs(value) > 0.25f){
				usingController = true;
				return value;
			}
		}
	}
	return 0.0f;
}

float Controller::axisPressed(const unsigned int& controllerId, const sf::Joystick::Axis& axis){
	return axisPosition(controllerId, axis) > 0.9f;
}