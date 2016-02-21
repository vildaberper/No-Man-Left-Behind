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

void Game::run(){
	sf::Clock clock;
	c::initialize();
	logger::timing("Constants initialized in " + to_string(clock.getElapsedTime().asSeconds()) + " seconds.");
	clock.restart();
	gi::initalize(window);
	gi::smoothCamera = true;
	logger::timing("Graphics interface initialized in " + to_string(clock.getElapsedTime().asSeconds()) + " seconds.");

	gi::renderWindow->setMouseCursorVisible(false);

	Level* level = NULL;

	Time lastTime;
	while(gi::startOfFrame()){
		if(!managerInitialized){
			gi::darken(1.0f);
			gi::endOfFrame();
			clock.restart();
			manager = new Manager();
			manager->initialize(window);
			logger::timing("Manager initialized in " + to_string(clock.getElapsedTime().asSeconds()) + " seconds.");
			controller = new Controller();
			controller->initialize(manager);
			managerInitialized = true;
			continue;
		}
		if(level == NULL){
			gi::darken(1.0f);
			gi::endOfFrame();
			clock.restart();
			level = new Level(manager, controller);
			level->begin();
			logger::timing("Level initialized in " + to_string(clock.getElapsedTime().asSeconds()) + " seconds.");
			clock.restart();
		}

		if(manager->inputManager->isPressed(Keyboard::Escape)){
			window->close();
		}

		Time time = clock.getElapsedTime();
		float dt = (time - lastTime).asSeconds();

		gi::collisionBoxes = manager->inputManager->isPressed(Keyboard::C);

		manager->tick(window, time, dt);

		window->clear();

		level->tick();

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