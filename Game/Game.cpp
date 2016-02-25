#include "Game.h"

#include <string>
#include <vector>
#include <iostream>

#include "World.h"
#include "Configuration.h"

using namespace sf;
using namespace std;
using namespace math;

Game::Game(){

}


Game::~Game(){

}

void Game::on(KeyboardEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(event.pressed()){
		switch(event.key()){
		case Keyboard::Escape:
			gi::renderWindow->close();
			break;
		case Keyboard::C:
			if(event.first()){
				gi::collisionBoxes = !gi::collisionBoxes;
			}
			break;
		case Keyboard::L:
			if(event.first()){
				gi::logAlwaysActive = !gi::logAlwaysActive;
			}
			break;
		}
	}
}

void Game::run(){
	sf::Clock clock;
	c::initialize();
	gi::initalize(window);
	gi::smoothCamera = true;

	gi::renderWindow->setMouseCursorVisible(false);

	Level* level = NULL;

	Time lastTime;
	while(gi::startOfFrame()){
		if(!managerInitialized){
			gi::darken(1.0f);
			gi::endOfFrame();
			manager = new Manager();
			manager->initialize(window);
			controller = new Controller();
			controller->initialize(manager);
			managerInitialized = true;
			inputListenerId = manager->inputManager->registerListener(this);
			continue;
		}
		if(level == NULL){
			gi::darken(1.0f);
			gi::endOfFrame();
			level = new Level(manager, controller);
			level->begin();
			clock.restart();
		}

		Time time = clock.getElapsedTime();
		float dt = (time - lastTime).asSeconds();

		manager->tick(window, time, dt);

		window->clear();

		level->tick();

		gi::drawLog();

		manager->menuManager->draw(time);

		lastTime = time;
		gi::endOfFrame();
	}
	delete level;
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete controller;
}