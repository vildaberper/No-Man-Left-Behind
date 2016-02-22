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
	//spawn = Vector(1550.0f, 2050.0f);
	//useTruck = false;

	spawn = Vector(-3000.0f, 440.0f);
	useTruck = true;

	Injured* in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 0); // soldier
	in->position = Vector(1644.000000f, -20.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "civil1", OPEN_WOUND, 1); // civil
	in->position = Vector(2079.000000f, -11.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 2); // soldier
	in->position = Vector(838.000000f, 6.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 0); // soldier
	in->position = Vector(1240.000000f, 6.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "civil2", OPEN_WOUND, 1); // civil
	in->position = Vector(2163.000000f, 374.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "general1", OPEN_WOUND, 2); // officer
	in->position = Vector(2156.000000f, 827.000000f);
	injured.push_back(in);


	in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 0); // soldier
	in->position = Vector(1762.000000f, 878.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 1); // soldier
	in->position = Vector(1343.000000f, 896.000000f);
	injured.push_back(in);

	in = new Injured();
	in->initialize(manager, "soldier1", OPEN_WOUND, 2); // soldier
	in->position = Vector(994.000000f, 907.000000f);
	injured.push_back(in);
	//

	state = TRUCKMOVING;
	fadeValue = 1.0f;
	timeBeforeBreak = 2.0f;

	world = new World(manager);
	world->load(c::worldDir.child(worldFileName));
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	for(Injured* inj : injured){
		world->addDrawable(inj, LAYER2);
	}

	if(player == NULL){
		player = new Player();
		player->initialize(manager);
		player->inventory = new Inventory(9);
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

	if(!useTruck){
		state = PLAYING;
	}

	player->inventory->put(ItemStack(PENICILLIN, 1));
	player->inventory->put(ItemStack(FORCEPS, 1));
	player->inventory->put(ItemStack(ALCOHOL, 1));
	player->inventory->put(ItemStack(MORPHINE, 1));
	player->inventory->put(ItemStack(SUTURE_KIT, 1));
	player->inventory->put(ItemStack(SCALPEL, 1));
	player->inventory->put(ItemStack(GAUZE, 33));

	si::playMusic("Level_1", "Level_01_Sketch_01", false, true, true);
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
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num4)){
		world->setTimeScale(8.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num5)){
		world->setTimeScale(16.0f);
	}

	if(manager->inputManager->isFirstPressed(sf::Keyboard::F)){
		si::playSound("truck.running", false);
	}
	if(manager->inputManager->isFirstPressed(sf::Keyboard::G)){
		si::playSound("truck.stopping", false);
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
			sf::FloatRect tr = truck->bounds(world->time());
			sf::FloatRect pr = player->getSprite(world->time())->getLocalBounds();
			player->position.x = tr.left + tr.width * 0.55f;
			player->position.y = tr.top + tr.height - pr.height * player->cb.offset.y;
			world->entities.push_back(player);
			world->collidables.push_back(player);

			Menu* invM = new Menu();
			invM->hidden = false;
			invM->type = HORIZONTAL;
			invM->position = Vector(5.0f, gi::TARGET_HEIGHT - 85.0f);
			invM->size = Vector(gi::TARGET_WIDTH - 10, 80);
			for(size_t i = 0; i < player->inventory->getSize(); i++){
				ItemStack is = player->inventory->content[i];
				MenuItem* ti = new MenuItem();
				ti->title = resourceToString(is.item.type) + " " + std::to_string(is.amount);
				ti->type = TEXTURE;
				ti->sprite = new sf::Sprite(*manager->spriteManager->getSprite("Resources." + resourceToString(is.item.type)));
				ti->closeOnClick = false;
				invM->items.push_back(ti);
			}
			manager->menuManager->menus["inventory"] = invM;
		}
		break;
	}
	case PLAYING:
	{
		player->velocity = controller->movement();
		world->tick();
		gi::cameraTargetX = player->position.x + player->getSprite(world->time())->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = player->position.y + player->getSprite(world->time())->getGlobalBounds().height / gi::dy() / 2;

		Vector camera = controller->camera();
		gi::cameraTargetX += camera.x * 250.0f;
		gi::cameraTargetY += camera.y * 250.0f;

		gi::camera(world->dt());
		world->render(player);

		// Inventory
		manager->menuManager->draw(world->time());

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