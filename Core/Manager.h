#pragma once

#include "AnimationManager.h"
#include "SpriteManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "SoundManager.h"
#include "MenuManager.h"

class Manager{
public:
	InputManager* inputManager;
	SoundManager* soundManager;
	SpriteManager* spriteManager;
	MenuManager* menuManager;
	MusicManager* musicManager;
	AnimationManager* animationManager;

	Manager(){
		inputManager = new InputManager();
		soundManager = new SoundManager();
		spriteManager = new SpriteManager();
		menuManager = new MenuManager(inputManager);
		musicManager = new MusicManager();
		animationManager = new AnimationManager();
	}
	~Manager(){
		delete inputManager;
		delete soundManager;
		delete spriteManager;
		delete menuManager;
		delete musicManager;
		delete animationManager;
	};

	void initialize(sf::RenderWindow* window){
		inputManager->initialize(window);
		soundManager->initialize(window);
		spriteManager->initialize(window);
		menuManager->initialize(window);
		musicManager->initialize(window);
		animationManager->initialize(window, spriteManager);
	}

	void finalize(sf::RenderWindow* window){
		inputManager->finalize(window);
		soundManager->finalize(window);
		spriteManager->finalize(window);
		menuManager->finalize(window);
		musicManager->finalize(window);
		animationManager->finalize(window);
	}

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){
		inputManager->tick(window, time, dt);
		soundManager->tick(window, time, dt);
		spriteManager->tick(window, time, dt);
		menuManager->tick(window, time, dt);
		musicManager->tick(window, time, dt);
		animationManager->tick(window, time, dt);
	}
};