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
	spawn = Vector(1550.0f, 2050.0f);
	useTruck = false;
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

	if(useTruck){
		truck = new Truck();
		truck->initialize(manager);
		truck->position = spawn;
		truck->position.y -= truck->getSprite(world->time())->getGlobalBounds().height * truck->cb.renderOffset / gi::dy();
		world->addDrawable(truck, LAYER2);
	}
	else{
		player->position = spawn;
		world->entities.push_back(player);
		world->collidables.push_back(player);
	}

	Injured* in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 0); // soldier
	in->position = Vector(1644.000000f, -20.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 1); // civil
	in->position = Vector(2079.000000f, -11.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 2); // soldier
	in->position = Vector(838.000000f, 6.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 0); // soldier
	in->position = Vector(1240.000000f, 6.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 1); // civil
	in->position = Vector(2163.000000f, 374.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 2); // officer
	in->position = Vector(2156.000000f, 827.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);


	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 0); // soldier
	in->position = Vector(1762.000000f, 878.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 1); // soldier
	in->position = Vector(1343.000000f, 896.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "testinjured", OPEN_WOUND, 2); // soldier
	in->position = Vector(994.000000f, 907.000000f);
	world->addDrawable(in, LAYER2);
	injured.push_back(in);

	if(!useTruck){
		state = PLAYING;
	}
}

void Level::tick(){
	if(controller->isPressed(PAUSE)){
		world->setPaused(!world->isPaused());
	}

	if(manager->inputManager->isFirstPressed(sf::Keyboard::Num0)){
		world->setTimeScale(0.5f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num1)){
		world->setTimeScale(1.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num2)){
		world->setTimeScale(2.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num3)){
		world->setTimeScale(4.0f);
	}

	if(fadeValue > 0.0f){
		fadeValue -= world->dt();
	}
	switch(state){
	case TRUCKMOVING:
	{
		world->tick();
		gi::cameraTargetX = truck->position.x + truck->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = truck->position.y + truck->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;
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

	if(firstFrame){
		gi::cameraX = gi::cameraTargetX;
		gi::cameraY = gi::cameraTargetY;
		firstFrame = false;
	}

	if(fadeValue > 0.0f){
		gi::darken(fadeValue);
	}
}

bool Level::done(){
	return false;
}