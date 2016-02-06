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
	logger::timing("Constants initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	fg.restart();
	gi::initalize(window);
	logger::timing("Graphics interface initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	manager = new Manager();
	manager->initialize(window);
	logger::timing("Manager initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	world = new World();
	file = File().child("world.txt");
	world->load(file, manager);
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	controller = new Controller();
	controller->initialize(manager);

	player = new Player();

	player->initialize(manager);

	window->setFramerateLimit(60);
	while (gi::startOfFrame()){
		world->tick();
		manager->tick(window, world->time(), world->dt());

		player->velocity = controller->movement();
		player->position += player->velocity * (world->dt() * 400.0f);
		gi::cameraX = player->position.x;
		gi::cameraY = player->position.y;

		switch (player->velocity.direction()){
		case XN:
			player->currentAnimation = player->nextAnimation = "left";
			break;
		case XP:
			player->currentAnimation = player->nextAnimation = "right";
			break;
		case ZERO:
			if (player->currentAnimation == "left"){
				player->currentAnimation = player->nextAnimation = "idleleft";
			}
			else if (player->currentAnimation == "right"){
				player->currentAnimation = player->nextAnimation = "idleright";
			}
			break;
		default:
			player->currentAnimation = player->nextAnimation = "idleleft";
			break;
		}

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