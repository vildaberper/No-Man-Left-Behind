#include "Level.h"

Level::Level(Manager* manager, Controller* controller){
	Level::manager = manager;
	Level::controller = controller;
}

Level::~Level(){
	delete world;
}

void Level::begin(){
	// Load level (levelFileName)
	worldFileName = "world.txt";
	spawn = Vector(100.0f, 250.0f);
	//

	state = TRUCKMOVING;
	fadeValue = 1.0f;
	timeBeforeBreak = 2.0f;

	world = new World(manager);
	world->load(c::worldDir.child(worldFileName));
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	player = new Player();
	player->initialize(manager);

	truck = new Truck();
	truck->initialize(manager);
	truck->position = spawn;
	truck->position.y -= truck->getSprite(world->time())->getGlobalBounds().height * truck->cb.renderOffset / gi::dy();
	world->addDrawable(truck, LAYER2);
}

void Level::tick(){
	if(fadeValue > 0.0f){
		fadeValue -= world->dt();
	}
	switch(state){
	case TRUCKMOVING:
	{
		world->tick();
		gi::cameraX = truck->position.x + truck->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraY = truck->position.y + truck->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		world->render(truck);

		if(world->time().asSeconds() > timeBeforeBreak){
			state = TRUCKBREAK;
			truck->targetspeed = 0.0f;
		}

		break;
	}
	case TRUCKBREAK:
	{
		world->tick();
		gi::cameraX = truck->position.x + truck->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraY = truck->position.y + truck->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		world->render(truck);

		if(truck->velocity.direction() == ZERO){
			state = PLAYING;
			player->position.x = truck->position.x + (truck->getSprite(world->time())->getGlobalBounds().width * 0.9f - player->getSprite(world->time())->getGlobalBounds().width / 2) / gi::dx();
			player->position.y = truck->position.y + (truck->getSprite(world->time())->getGlobalBounds().height - player->getSprite(world->time())->getGlobalBounds().height * player->cb.renderOffset) / gi::dy();
			world->entities.push_back(player);
			world->collidables.push_back(player);
		}

		break;
	}
	case PLAYING:
	{
		player->velocity = controller->movement();
		world->tick();
		gi::cameraX = player->position.x + player->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraY = player->position.y + player->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		world->render(player);
		break;
	}
	}
	if(fadeValue > 0.0f){
		gi::darken(fadeValue);
	}
}

bool Level::done(){
	return false;
}