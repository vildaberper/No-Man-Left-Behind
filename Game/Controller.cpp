#include "Controller.h"

Controller::Controller(){

}

Controller::~Controller(){

}

void Controller::initialize(Manager* m){
	im = m->inputManager;
}

Vector Controller::movement(){
	if (sf::Joystick::isConnected(0)){
		if (sf::Joystick::hasAxis(0, sf::Joystick::X) && sf::Joystick::hasAxis(0, sf::Joystick::Y)){
			Vector joystick = Vector(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y)) / 100.0f;
			if (joystick.length() > 0.25f){
				return joystick;
			}
		}
	}
	return Vector(
		(isPressed(LEFT) ? -1.0f : 0.0f) + (isPressed(RIGHT) ? 1.0f : 0.0f),
		(isPressed(UP) ? -1.0f : 0.0f) + (isPressed(DOWN) ? 1.0f : 0.0f)
		).norm();
}

bool Controller::isPressed(const Command& c){
	switch (c){
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
	}
	return false;
}