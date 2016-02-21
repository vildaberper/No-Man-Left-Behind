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

	if(player == NULL){
		player = new Player();
		player->initialize(manager);
	}

	truck = new Truck();
	truck->initialize(manager);
	truck->position = spawn;
	truck->position.y -= truck->getSprite(world->time())->getGlobalBounds().height * truck->cb.renderOffset / gi::dy();
	world->addDrawable(truck, LAYER2);

	Injured* in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 0);
	in->position = Vector(2000.0f, 500.0f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 1);
	in->position = Vector(2300.0f, 500.0f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 2);
	in->position = Vector(2600.0f, 500.0f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);
}

void Level::tick(){
	if(fadeValue > 0.0f){
		fadeValue -= world->dt();
	}
	switch(state){
	case TRUCKMOVING:
	{
		world->tick();
		gi::cameraTargetX = truck->position.x + truck->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = truck->position.y + truck->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		if(firstFrame){
			gi::cameraX = gi::cameraTargetX;
			gi::cameraY = gi::cameraTargetY;
			firstFrame = false;
		}
		gi::camera(world->dt());
		world->render(truck);

		if(world->time().asSeconds() > timeBeforeBreak){
			state = TRUCKBREAKING;
			truck->targetspeed = 0.0f;
		}
		break;
	}
	case TRUCKBREAKING:
	{
		world->tick();
		gi::cameraTargetX = truck->position.x + truck->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = truck->position.y + truck->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		gi::camera(world->dt());
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
		gi::cameraTargetX = player->position.x + player->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = player->position.y + player->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
		gi::camera(world->dt());
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