#pragma once

#include "SpriteManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "MenuManager.h"

class Manager{
public:
	InputManager* inputManager;
	SoundManager* soundManager;
	SpriteManager* spriteManager;
	MenuManager* menuManager;

	Manager(){
		inputManager = new InputManager();
		soundManager = new SoundManager();
		spriteManager = new SpriteManager();
		menuManager = new MenuManager(inputManager);
	}
	~Manager(){
		delete inputManager;
		delete soundManager;
		delete spriteManager;
	};

	void initialize(sf::RenderWindow* window){
		inputManager->initialize(window);
		soundManager->initialize(window);
		spriteManager->initialize(window);
		menuManager->initialize(window);
	}

	void finalize(sf::RenderWindow* window){
		inputManager->finalize(window);
		soundManager->finalize(window);
		spriteManager->finalize(window);
		menuManager->finalize(window);
	}

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){
		inputManager->tick(window, time, dt);
		soundManager->tick(window, time, dt);
		spriteManager->tick(window, time, dt);
		menuManager->tick(window, time, dt);
	}
};