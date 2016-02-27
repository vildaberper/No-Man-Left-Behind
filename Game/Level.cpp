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
	timeBeforeBreak = 2.0f;

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
		world->addDrawable(player, LAYER2);
	}

	if(!useTruck){
		state = PLAYING;
	}

	player->inventory->put(ItemStack(PENICILLIN, 16));
	player->inventory->put(ItemStack(FORCEPS, 16));
	player->inventory->put(ItemStack(ALCOHOL, 16));
	player->inventory->put(ItemStack(MORPHINE, 16));
	player->inventory->put(ItemStack(SUTURE_KIT, 16));
	player->inventory->put(ItemStack(SCALPEL, 16));
	player->inventory->put(ItemStack(GAUZE, 16));

	journal = new Animatable();
	journal->setAnimationType(STATES);
	journal->applyAnimation(manager, "journal");
	journal->position = Vector(0.0,0.0);
	journal->scale = 0.7f;
	journal->viewRelative = true;
	world->addDrawable(journal, LAYER4);

	si::playMusic("level1", "main", true, true, true);
	//si::playMusic("WIND", "Vind_Inside", true, false, true);
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
		world->setTimeScale(3.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num4)){
		world->setTimeScale(4.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num5)){
		world->setTimeScale(5.0f);
	}
	else if(manager->inputManager->isFirstPressed(sf::Keyboard::Num6)){
		world->setTimeScale(16.0f);
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
			if(useTruck){
				world->addDrawable(player, LAYER2);
			}

			Menu* invM = new Menu();
			invM->hidden = false;
			invM->type = HORIZONTAL;
			invM->position = Vector(5.0f, gi::TARGET_HEIGHT - 85.0f);
			invM->size = Vector(gi::TARGET_WIDTH - 10, 80);
			mis = &invM->items;
			for(size_t i = 0; i < player->inventory->getSize(); i++){
				MenuItem* ti = new MenuItem();
				ti->closeOnClick = false;
				mis->push_back(ti);
			}
			updateInventoryMenu();
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

		closest = nearestInjured(150.0f);

		if(controller->isPressed(LB)){
			selectedSlot--;
			updateInventoryMenu();
		}
		if(controller->isPressed(RB)){
			selectedSlot++;
			updateInventoryMenu();
		}

		if(closest != NULL && controller->isPressed(INTERACT)){
			closest->use(player->inventory->at(selectedSlot));
			updateInventoryMenu();
		}
		target = closest != NULL && !closest->isHealed() ? 0.0f : dist;

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

	Vector cv = Vector(0.0f, target - actual);
	if(cv.length() > 0.0f){
		cv *= world->dt() * (d - (d / cv.length()));
		actual += cv.y;
	}

	journal->position.x = 50;
	journal->position.y = 50;
	journal->position.y -= actual;
}

bool Level::done(){
	return false;
}

void Level::updateInventoryMenu(){
	selectedSlot = math::range(selectedSlot, player->inventory->getSize() - 1);
	for(size_t i = 0; i < player->inventory->getSize(); i++){
		ItemStack is = player->inventory->content[i];
		MenuItem* ti = (*mis)[i];
		if(is.amount > 0){
			ti->title = resourceToString(is.item.type) + " " + std::to_string(is.amount);
			ti->type = TEXTURE;
			ti->sprite = new sf::Sprite(*manager->spriteManager->getSprite("Resources." + resourceToString(is.item.type)));
		}
		else{
			ti->title = "";
			ti->type = TEXT;
		}
		ti->highlight = i == selectedSlot;
	}
}

Injured* Level::nearestInjured(const float& maxDistance){
	Injured* i = NULL;
	float d = maxDistance;

	sf::FloatRect pfr = player->bounds(world->time());

	for(Injured* inj : injured){
		if(!inj->isAlive()){
			continue;
		}
		sf::FloatRect ifr = inj->bounds(world->time());
		float distance = math::distance(pfr.left + pfr.width / 2, pfr.top + pfr.height / 2, ifr.left + ifr.width / 2, ifr.top + ifr.height / 2);
		if(d > distance){
			d = distance;
			i = inj;
		}
	}
	return i;
}