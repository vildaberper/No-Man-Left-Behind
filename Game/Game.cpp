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
	sf::Clock fg;
	c::initialize();
	logger::timing("Constants initialized in " + to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	fg.restart();
	gi::initalize(window);
	logger::timing("Graphics interface initialized in " + to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	manager = new Manager();
	manager->initialize(window);
	logger::timing("Manager initialized in " + to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	world = new World(manager);
	file = File().child("world.txt");
	world->load(file);
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	controller = new Controller();
	controller->initialize(manager);

	player = new Player();

	player->initialize(manager);
	world->entities.push_back(player);
	world->collidables.push_back(player);

	gi::collisionBoxes = true;

	window->setFramerateLimit(60);
	while(gi::startOfFrame()){
		player->velocity = controller->movement() * (400.0f * (manager->inputManager->isPressed(Keyboard::LShift) ? 10.0f : 1.0f));
		player->cb.shouldCollide = !manager->inputManager->isPressed(Keyboard::E);

		world->tick();
		manager->tick(window, world->time(), world->dt());

		gi::cameraX = player->position.x;
		gi::cameraY = player->position.y;

		window->clear();

		world->render(player);

		manager->menuManager->draw(world->time());

		gi::endOfFrame();
	}
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
	delete controller;
	delete player;
}