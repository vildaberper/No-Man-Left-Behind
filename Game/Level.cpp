#include "Level.h"

Level::Level(Manager* manager, Controller* controller, JournalManager* jmanager){
	Level::manager = manager;
	Level::controller = controller;
	Level::jmanager = jmanager;
}

Level::~Level(){
	delete world;
	si::stopMusic(introId);
	si::stopMusic(mainId);
}

void Level::load(File& file){
	Configuration c;
	sf::Clock cl;
	c.load(file);
	logger::timing("Level configuration loaded in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	cl.restart();

	worldFileName = c.stringValue("general.worldFileName");
	spawn = c.floatVector("general.spawn");
	useTruck = c.boolValue("general.useTruck");
	timeBeforeBreak = c.floatValue("general.timeBeforeBreak");

	musicIntro = c.stringValue("music.intro");
	musicMain = c.stringValue("music.main");

	journals = c.stringVector("journals");
	size_t m = c.children("injuredPositions").size();
	for(size_t i = 0; i < m; i++){
		injuredPositions.push_back(c.floatVector("injuredPositions." + std::to_string(i)));
	}

	logger::timing("Level configuration applied in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");

	logger::info("Level loaded: " + file.parent().name() + "\\" + file.name());
}

void Level::save(File& file){
	Configuration c;
	// TODO
	c.save(file);
}

void Level::begin(){
	for(size_t i = 0; i < journals.size(); i++){
		if(injuredPositions.size() == 0){
			logger::warning("Not enough spawn positions for injured.");
			break;
		}

		Journal* j = jmanager->getJournal(journals[i]);
		size_t pos = random::random(injuredPositions.size() - 1);
		size_t type = random::random(j->injured.size() - 1);

		Injured* in = new Injured();
		in->initialize(manager, j->injured[type], j);
		in->position = injuredPositions[pos];
		injured.push_back(in);
		injuredPositions.erase(injuredPositions.begin() + pos);
	}

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
	}

	if(useTruck){
		truck = new Truck();
		truck->initialize(manager);
		truck->position = spawn;
		truck->position.y -= truck->getSprite(world->time())->h() * truck->scale * truck->cb.renderOffset / gi::dy();
		world->addDrawable(truck, LAYER2);
	}
	else{
		player->position = spawn;
		player->position.y -= player->getSprite(world->time())->h() * player->scale * player->cb.renderOffset / gi::dy();
		world->addDrawable(player, LAYER2);
		playerInventory->menu->hidden = false;
	}

	if(!useTruck){
		state = PLAYING;
	}

	playerInventory = new PlayerInventory(manager, 9);
	playerInventory->put(ItemStack(PENICILLIN, 16));
	playerInventory->put(ItemStack(FORCEPS, 16));
	playerInventory->put(ItemStack(ALCOHOL, 16));
	playerInventory->put(ItemStack(MORPHINE, 16));
	playerInventory->put(ItemStack(SUTURE_KIT, 16));
	playerInventory->put(ItemStack(SCALPEL, 16));
	playerInventory->put(ItemStack(GAUZE, 16));
	playerInventory->update();

	journal = new Animatable();
	journal->setAnimationType(STATES);
	journal->applyAnimation(manager, "journal");
	journal->position = Vector(0.0,0.0);
	journal->scale = 0.7f;
	journal->viewRelative = true;
	world->addDrawable(journal, LAYER4);

	//introId = si::playMusic(musicIntro, true, true, true);
	mainId = si::playMusic(musicMain, true, true, true);
	// music queue
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
		gi::cameraTargetX = truck->position.x + truck->getSprite(world->time())->sprite()->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = truck->position.y + truck->getSprite(world->time())->sprite()->getGlobalBounds().height / gi::dy() / 2;
		gi::camera(world->dt());
		world->render();

		if(world->time().asSeconds() > timeBeforeBreak){
			state = TRUCKBREAKING;
			truck->targetspeed = 0.0f;
		}
		break;
	}
	case TRUCKBREAKING:
	{
		world->tick();
		gi::cameraTargetX = truck->position.x + truck->getSprite(world->time())->sprite()->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = truck->position.y + truck->getSprite(world->time())->sprite()->getGlobalBounds().height / gi::dy() / 2;
		gi::camera(world->dt());
		world->render();

		if(truck->velocity.direction() == ZERO){
			state = PLAYING;
			sf::FloatRect tr = truck->bounds(world->time());
			sf::FloatRect pr = player->getSprite(world->time())->sprite()->getLocalBounds();
			player->position.x = tr.left + tr.width * 0.55f;
			player->position.y = tr.top + tr.height - pr.height * player->cb.offset.y;
			world->addDrawable(player, LAYER2);
			playerInventory->menu->hidden = false;
		}
		break;
	}
	case PLAYING:
	{
		player->velocity = controller->movement();
		world->tick();
		gi::cameraTargetX = player->position.x + player->getSprite(world->time())->sprite()->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = player->position.y + player->getSprite(world->time())->sprite()->getGlobalBounds().height / gi::dy() / 2;

		Vector camera = controller->camera();
		gi::cameraTargetX += camera.x * 250.0f;
		gi::cameraTargetY += camera.y * 250.0f;

		if(closest != NULL){
			closest->highlight = false;
		}
		closest = nearestInjured(150.0f);
		if(closest != NULL){
			closest->highlight = true;
		}

		if(controller->isPressed(LB)){
			playerInventory->selectedSlot--;
			playerInventory->update();
		}
		if(controller->isPressed(RB)){
			playerInventory->selectedSlot++;
			playerInventory->update();
		}

		if(closest != NULL && controller->isPressed(INTERACT)){
			closest->use(playerInventory->selectedItem());
			playerInventory->update();
		}
		target = closest != NULL && !closest->isHealed() ? 0.0f : dist;

		gi::camera(world->dt());
		world->render();

		manager->menuManager->draw(world->time());

		playerInventory->render();

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
	journal->position.y = 50 - actual;
	journal->movedY = true;

	if(closest != NULL){
		gi::draw(closest->journal->lines, (journal->position.x + 100) * gi::dxiz(), (journal->position.y + 240) * gi::dyiz(), 430 * gi::dxiz(), 600 * gi::dyiz());
	}
}

bool Level::done(){
	return manager->inputManager->isFirstPressed(sf::Keyboard::Q);
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