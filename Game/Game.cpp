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
	world = new World();
	file = File().child("world.txt");
	world->load(file, manager);
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	controller = new Controller();
	controller->initialize(manager);

	player = new Player();

	player->initialize(manager);

	ProgressBar* progressbar = new ProgressBar();
	progressbar->position = Vector(100, 100);
	progressbar->size = Vector(500.0f, 100.0f);
	progressbar->progress = 0.5f;
	progressbar->bleft = manager->spriteManager->getSprite("Floortiles.Stone");

	
	window->setFramerateLimit(0);
	while (gi::startOfFrame()){
		world->tick();
		manager->tick(window, world->time(), world->dt());

		

		player->velocity = controller->movement() * 400.0f;
		player->tick(world->time(), world->dt());
		gi::cameraX = player->position.x;
		gi::cameraY = player->position.y;

		window->clear();

		world->render(player);

		gi::draw(progressbar, world->time());

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