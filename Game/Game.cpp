#include "Game.h"

#include <string>
#include <vector>
#include <iostream>

#include "World.h"
#include "Cursor.h"
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
	gc::initialize();
	gi::initalize(window);
	gi::smoothCamera = true;

	random::initialize();

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
			gi::cursor = new Cursor(manager, "cursor");
			gi::showCursor = true;
			jmanager = new JournalManager();

			playerInventory = new PlayerInventory(controller, manager, gc::inventorySize);
			handBook = new Handbook(manager);
			continue;
		}

		Time time = clock.getElapsedTime();
		float dt = (time - lastTime).asSeconds();

		manager->tick(window, time, dt);

		window->clear();

		switch(state){
		case MAIN_MENU:
			state = LEVEL;
			break;
		case LEVEL:
			if(level == NULL){
				gi::darken(1.0f);
				gi::endOfFrame();
				level = new Level(manager, controller, jmanager);
				level->playerInventory = playerInventory;
				level->handBook = handBook;
				level->load(gc::levelDir.child(gc::levelProgression[currentLevel] + ".txt"));
				level->begin();
				clock.restart();
				continue;
			}
			level->tick();

			if(level->done()){
				currentLevel++;
				if(currentLevel >= gc::levelProgression.size()){
					delete level;
					state = COMPLETE;
					continue;
				}
				else{
					gi::darken(1.0f);
					gi::endOfFrame();
					delete level;
					level = new Level(manager, controller, jmanager);
					level->playerInventory = playerInventory;
					level->handBook = handBook;
					level->load(gc::levelDir.child(gc::levelProgression[currentLevel] + ".txt"));
					level->begin();
					clock.restart();
					continue;
				}
			}

			break;
		case COMPLETE:
			break;
		}

		gi::drawLog();

		lastTime = time;
		gi::endOfFrame();
	}
	manager->finalize(window);
	gi::finalize();
	delete controller;
	delete manager;
	delete jmanager;
}