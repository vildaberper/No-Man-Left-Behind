#pragma once

#include "AnimationManager.h"
#include "CollisionManager.h"
#include "SpriteManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "SoundManager.h"
#include "MenuManager.h"
#include "SI.h"

class Manager{
public:
	InputManager* inputManager;
	SoundManager* soundManager;
	MusicManager* musicManager;
	SpriteManager* spriteManager;
	MenuManager* menuManager;
	AnimationManager* animationManager;
	CollisionManager* collisionManager;

	Manager(){
		inputManager = new InputManager();
		soundManager = new SoundManager();
		musicManager = new MusicManager();
		spriteManager = new SpriteManager();
		menuManager = new MenuManager(inputManager);
		animationManager = new AnimationManager();
		collisionManager = new CollisionManager();
	}
	~Manager(){
		delete inputManager;
		delete soundManager;
		delete musicManager;
		delete spriteManager;
		delete menuManager;
		delete animationManager;
		delete collisionManager;
	};

	void initialize(sf::RenderWindow* window){
		inputManager->initialize(window);
		soundManager->initialize(window);
		musicManager->initialize(window);
		spriteManager->initialize(window);
		menuManager->initialize(window);
		animationManager->initialize(window, spriteManager);
		collisionManager->initialize(window);
		si::initalize(soundManager, musicManager);
	}

	void finalize(sf::RenderWindow* window){
		inputManager->finalize(window);
		soundManager->finalize(window);
		musicManager->finalize(window);
		spriteManager->finalize(window);
		menuManager->finalize(window);
		animationManager->finalize(window);
		collisionManager->finalize(window);
		si::finalize();
	}

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){
		inputManager->tick(window, time, dt);
		soundManager->tick(window, time, dt);
		musicManager->tick(window, time, dt);
		spriteManager->tick(window, time, dt);
		menuManager->tick(window, time, dt);
		animationManager->tick(window, time, dt);
		collisionManager->tick(window, time, dt);
	}
};