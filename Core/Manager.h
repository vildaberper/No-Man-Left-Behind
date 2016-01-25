#pragma once

#include "InputManager.h"

class Manager{
public:
	InputManager* inputManager;

	Manager(){
		inputManager = new InputManager();
	}
	~Manager(){
		delete inputManager;
	};

	void initialize(sf::RenderWindow* window){
		inputManager->initialize(window);
	}

	void finalize(sf::RenderWindow* window){
		inputManager->finalize(window);
	}

	void tick(sf::RenderWindow* window, clock_t time, float dt){
		inputManager->tick(window, time, dt);
	}
};