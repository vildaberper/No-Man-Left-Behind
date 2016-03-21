#include "Level.h"

float stepDistance = 5.0f;

Level::Level(Manager* manager, Controller* controller, JournalManager* jmanager, CursorSet* cursorSet){
	Level::manager = manager;
	Level::controller = controller;
	Level::jmanager = jmanager;
	Level::cursorSet = cursorSet;
}

Level::~Level(){
	manager->inputManager->unregisterListener(listenerId);
	delete world;
	stopMusic();
	delete resourceUseBar;
}

void Level::stopMusic(){
	si::stopMusic(introId);
	si::stopMusic(mainId);
	si::stopSound(armyMarchId);
	si::stopSound(armyScreamId);
}

void Level::load(File& file){
	levelFileName = file.name();
	Configuration c;
	sf::Clock cl;
	c.load(file);
	logger::timing("Level configuration loaded in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	cl.restart();

	worldFileName = c.stringValue("general.worldFileName");
	timeBeforeBreak = c.floatValue("general.timeBeforeBreak");
	timer = sf::seconds(c.floatValue("general.timer"));

	musicIntro = c.stringValue("music.intro");
	musicMain = c.stringValue("music.main");

	journals = c.stringVector("journals");
	size_t m = c.children("injuredPositions").size();
	for(size_t i = 0; i < m; i++){
		injuredPositions.push_back(c.floatVector("injuredPositions." + std::to_string(i)));
	}

	m = c.children("injured").size();
	for(size_t i = 0; i < m; i++){
		std::string id = std::to_string(i);
		std::string jn = c.stringValue("injured." + id + ".journal");
		if(usedJournals.count(journals[i]) == 0){
			usedJournals.insert(jn);
			Journal* j = jmanager->getJournal(jn);
			size_t type = random::randomInt(j->injured.size() - 1);

			Injured* in = new Injured();
			in->initialize(manager, j->injured[type], j);
			in->position = c.floatVector("injured." + id + ".position");
			injured.push_back(in);
		}
	}

	warSoundLower = c.floatValue("sound.warSoundLower");
	warSoundUpper = c.floatValue("sound.warSoundUpper");
	warSoundVolume = c.floatValue("sound.warSoundVolume");
	if(warSoundVolume != 0.0f){
		nextWarSound = sf::seconds(0.5f + random::randomFloat(warSoundLower, warSoundUpper));
	}

	logger::timing("Level configuration applied in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");

	logger::info("Level loaded: " + file.parent().name() + "\\" + file.name());
}

void Level::save(File& file){
	Configuration c;
	// TODO
	c.save(file);
}

void Level::begin(const float& stress, const float& timeBonus){
	Level::stress = stress;
	if(timer.asMilliseconds() > 0){
		timer += sf::seconds(timeBonus);
	}

	world = new World(manager);
	world->load(c::worldDir.child(worldFileName));

	timerHud = new Animatable();
	timerHud->setAnimationType(STATES);
	timerHud->applyAnimation(manager, "timer");
	timerHud->scale = 0.5f;
	timerHud->position = Vector(gi::TARGET_WIDTH - timerHud->getSprite(sf::milliseconds(0))->w() * timerHud->scale, 0.0);
	timerHud->viewRelative = true;

	for(drawable::Drawable* d : world->drawables[LAYER2]){
		if(
			(d->reference == "Characters.Ilse")
			){
			//logger::info("Found player spawn position (" + d->position.toString() + ").");
			spawn = d->position;
			d->kill();
			useTruck = false;
		}
		else if(
			(d->reference == "Characters.Truck")
			){
			//logger::info("Found truck spawn position (" + d->position.toString() + ").");
			truckspawn = d->position;
			d->kill();
		}
		else if(
			(d->reference.size() > 11 && d->reference.substr(0, 11) == "Characters.")
			){
			//logger::info("Found injured spawn position (" + d->position.toString() + ").");
			injuredPositions.push_back(d->position);
			d->kill();
		}
		else if(
			/*(d->reference.size() >= 9 && d->reference.substr(0, 9) == "Props.Box")
			||*/ (d->reference.size() >= 17 && d->reference.substr(0, 17) == "Props.Medical Box")
			|| (d->reference.size() >= 21 && d->reference.substr(0, 21) == "Props.Medical Cabinet")
			){
			//logger::info("Found resource box " + d->reference + " (" + d->position.toString() + ").");
			resourceBoxes.push_back(new ResourceBox(controller, manager, 7, playerInventory, d));
		}
		else if(d->reference.size() > 10 && d->reference.substr(0, 10) == "Resources."){
			std::string r = d->reference.substr(10);
			ItemStack is = ItemStack(parseResource(r), 1);
			//logger::info("Found resource " + r + " (" + d->position.toString() + ").");
			resources.push_back(ResourcePair(d, is));
		}
	}

	std::vector<Journal*> js;
	for(size_t i = 0; i < journals.size(); i++){

		if(journals[i].find_first_of('.') != std::string::npos){
			if(usedJournals.count(journals[i]) == 0){
				js.push_back(jmanager->getJournal(journals[i]));
			}
		}
		else{
			for(std::string j_ : jmanager->getJournals(journals[i])){
				if(usedJournals.count(j_) == 0){
					js.push_back(jmanager->getJournal(j_));
				}
			}
		}
	}
	for(Journal* j : js){
		if(injuredPositions.size() == 0){
			logger::warning("Not enough spawn positions for the injured.");
			break;
		}

		size_t pos = random::randomInt(injuredPositions.size() - 1);
		size_t type = random::randomInt(j->injured.size() - 1);

		Injured* in = new Injured();
		in->initialize(manager, j->injured[type], j);
		in->position = injuredPositions[pos];
		injured.push_back(in);
		injuredPositions.erase(injuredPositions.begin() + pos);
	}

	state = TRUCKMOVING;
	fadeValue = 1.0f;

	for(Injured* inj : injured){
		world->addDrawable(inj, LAYER2);
	}

	if(player == nullptr){
		player = new Player();
		player->initialize(manager);
		player->speed = gc::playerSpeedLower + (gc::playerSpeedUpper - gc::playerSpeedLower) * stress;
		CoreSprite* cs = player->getSprite(sf::milliseconds(0));
		relativeOffset = player->scale * (-(cs->h() / 2.0f) + cs->h() * player->cb.offset.y + cs->h() * player->cb.size.y / 2.0f);
	}

	truck = new Truck();
	truck->initialize(manager);
	if(useTruck){
		truck->position = truckspawn;
		manager->soundManager->relative = truck;
	}
	else{
		truck->speed = 0.0f;
		truck->targetspeed = 0.0f;
		truck->position = truckspawn;

		player->position = spawn;
		world->addDrawable(player, LAYER2);
		playerInventory->menu->hidden = false;
		manager->soundManager->relative = player;
	}
	world->addDrawable(truck, LAYER2);

	if(!useTruck){
		state = PLAYING;
	}

	playerInventory->update();

	journal = new Animatable();
	journal->setAnimationType(STATES);
	journal->applyAnimation(manager, "journal");
	journal->position = Vector(0.0, 0.0);
	journal->scale = 0.85f;
	journal->viewRelative = true;
	journal->scaleRelative = false;
	dist /= gc::zoomLevel;
	skull = manager->spriteManager->getSprite("timer.skull");

	if(musicIntro.length() > 0){
		introId = si::playMusic(musicIntro, true, false, false);
		mainId = si::queueMusic(introId, musicMain, false, true, true);
	}
	else if(musicMain.length() > 0){
		mainId = si::playMusic(musicMain, true, true, true);
	}

	std::vector<ItemStack> iss;
	for(Injured* inj : injured){
		for(Resource r : inj->journal->requirements){
			iss.push_back(ItemStack(r, 1));
		}
	}
	if(iss.size() > 0){
		for(size_t i = 0; i < extraResources; i++){
			size_t is = random::randomInt(iss.size() - 1);
			iss[is].amount++;
		}
		for(size_t i = 0; i < resources.size(); i++){
			ItemStack is = ItemStack(resources[i].second.item.type, resources[i].second.amount);

			for(ItemStack& rem : iss){
				if(rem.item.type == is.item.type){
					if(rem.amount >= is.amount){
						rem.amount -= is.amount;
						is.amount = 0;
					}
					else{
						is.amount -= rem.amount;
						rem.amount = 0;
					}
				}
				if(is.amount == 0){
					break;
				}
			}
			if(is.amount != 0){
				logger::warning("Resource found in world is not part of journal requirements (" + resourceToString(is.item.type) + ").");
			}
		}
	}

	bool empty = iss.size() == 0;
	int checkIndex = 0;
	while(!empty){
		for(ItemStack& is : iss){
			if(is.amount > 0){
				size_t rb = random::randomInt(resourceBoxes.size() - 1);
				resourceBoxes[rb]->put(is);
			}
		}

		empty = true;
		for(ItemStack is : iss){
			empty = is.amount > 0 ? false : empty;
		}
		checkIndex++;
		if(checkIndex >= 1000){
			logger::warning("Not enough room for resources!");
			break;
		}
	};

	for(ResourceBox* rb : resourceBoxes){
		rb->update();
		world->addDrawable(rb, LAYER2);
	}

	resourceUseBar = new ProgressBar();
	resourceUseBar->customColors = true;
	resourceUseBar->bgColor = sf::Color(185, 185, 185, 155);
	resourceUseBar->oColor = sf::Color(5, 5, 5, 255);
	resourceUseBar->size = Vector(100.0f, 20.0f);

	listenerId = manager->inputManager->registerListener(this);

	gi::zoom(gc::zoomLevel);
}

void Level::tick(){
	gi::showCursor = !controller->usingController;

	if(wasUsingController != controller->usingController){
		playerInventory->update();
		if(closestBox != nullptr){
			closestBox->update();
		}
		wasUsingController = controller->usingController;
	}

	if(controller->isPressed(PAUSE)){
		if(handBook->isOpen()){
			handBook->close();
		}
		else{
			world->setPaused(!world->isPaused());
			if(closestBox != nullptr){
				closestBox->menu->hidden = true;
				si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
			}
		}
	}

	if(gc::developerMode){
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

		if(manager->inputManager->isFirstPressed(sf::Keyboard::Return)){
			completeState = IN_TRUCK;
		}
	}

	if(fadeValue > 0.0f && completeState == IN_GAME){
		fadeValue -= world->dt() / 2.0f;
	}
	if(fadeValue < 1.0f && completeState != IN_GAME){
		fadeValue += world->dt() / 2.0f;
	}

	if(nextWarSound.asMilliseconds() != 0 && time > nextWarSound){
		bool end = timer.asMilliseconds() != 0 && ((timer - time).asSeconds() <= gc::armyScreamLength);
		nextWarSound = time + sf::seconds(random::randomFloat((end ? 0.2f : warSoundLower), (end ? 0.5f : warSoundUpper)));
		si::playRandomSoundV(nullptr, "war", (end ? 0.5f : warSoundVolume));
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
			manager->soundManager->relative = player;
		}
		break;
	}
	case PLAYING:
	{
		if(timer.asMilliseconds() > 0 && time > timer){
			completeState = TIME_RAN_OUT;
		}

		canControl = canUseResource = completeState == IN_GAME;

		float resourceUseProgress = 1.0f;
		if((resourceUseInjured != nullptr && !resourceUseInjured->isDead()) && resourceUsedTimer.asMilliseconds() > 0){
			resourceUseProgress = (time - resourceUsedTime) / (resourceUsedTimer + (resourceUsedTimer * gc::stressIncreaseUseTime * (1.0f - stress)));
		}

		if(resourceUsedTime.asMilliseconds() != 0 && resourceUseProgress < 1.0f){
			usingResource = true;
			canControl = false;
			canUseResource = gc::useWhileTimer;
			resourceUseBar->progress = resourceUseProgress;
		}
		else{
			if(resourceUseInjured != nullptr){
				usingResource = false;
				resourceUseInjured->updateAnimation();
				resourceUsedTimer = sf::milliseconds(0);
				resourceUseInjured = nullptr;
			}
		}

		if(hasUsedResource){
			nearTruck_ = nearTruck(320.0f);
			truck->highlight = nearTruck_;
		}

		gi::cameraTargetX = player->position.x + player->getSprite(world->time())->sprite()->getGlobalBounds().width / gi::dx() / 2;
		gi::cameraTargetY = player->position.y + player->getSprite(world->time())->sprite()->getGlobalBounds().height / gi::dy() / 2;
		if(canControl){
			player->velocity = controller->movement();

			Vector camera = controller->camera();
			gi::cameraTargetX += camera.x *= 250.0f;
			gi::cameraTargetY += camera.y *= 250.0f;
			gi::relativeOffset = camera * -1.0f;
			gi::relativeOffset.y += relativeOffset;
		}
		else{
			player->velocity = Vector(0.0f, 0.0f);
		}

		world->tick();
		time += sf::seconds(world->dt());

		if(closest != nullptr){
			closest->highlight = false;
		}
		closest = nearestInjured(150.0f);
		if(closest != nullptr){
			closest->highlight = true;
		}

		ResourceBox* cb = nearestResourceBox(150.0f);
		if(cb != nullptr){
			if(closestBox != nullptr){
				if(cb != closestBox){
					closestBox->highlight = false;
					if(!closestBox->menu->hidden){
						closestBox->menu->hidden = true;
						si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
					}
				}
			}
			closestBox = cb;
			closestBox->highlight = true;
		}
		else{
			if(closestBox != nullptr){
				closestBox->highlight = false;
				if(!closestBox->menu->hidden){
					closestBox->menu->hidden = true;
					si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
				}
				closestBox = cb;
			}
		}
		if(closestBox != nullptr && !closestBox->menu->hidden && !closestBox->menu->hidden && !handBook->isClosed()){
			closestBox->menu->hidden = true;
			si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
		}

		if(canControl && controller->isPressed(Command::HANDBOOK)){
			if(handBook->isOpen()){
				handBook->close();
			}
			else if(handBook->isClosed()){
				handBook->open();
			}
		}
		if(canControl && controller->isPressed(Command::BACK)){
			if(handBook->isOpen()){
				handBook->close();
			}
			if(closestBox != nullptr && !closestBox->menu->hidden){
				closestBox->menu->hidden = true;
				si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
			}
		}

		if(canControl && handBook->isOpen()){
			if(controller->isPressed(LB)){
				handBook->turnLeft();
			}
			if(controller->isPressed(RB)){
				handBook->turnRight();
			}
		}

		if(canUseResource && controller->isPressed(LB)){
			playerInventory->selectedSlot--;
			playerInventory->update();
		}
		if(canUseResource && controller->isPressed(RB)){
			playerInventory->selectedSlot++;
			playerInventory->update();
		}

		if(canUseResource && closestBox != nullptr && !closestBox->menu->hidden){
			if(controller->isPressed(LT)){
				closestBox->selectedSlot--;
				closestBox->update();
			}
			if(controller->isPressed(RT)){
				closestBox->selectedSlot++;
				closestBox->update();
			}
		}

		if(controller->isPressed(INTERACT) && handBook->isClosed()){
			if(canUseResource && closest != nullptr){
				if(!hasUsedResource){
					hasUsedResource = playerInventory->selectedItem().amount > 0;
				}
				if(playerInventory->selectedItem().amount > 0){
					if(closest->use(playerInventory->selectedItem())){
						resourceUseInjured = closest;
						if(gc::useTimerReset){
							resourceUsedTime = time;
							resourceUsedTimer = getResourceUseTime(playerInventory->itemInHand.item.type);
						}
						else{
							if(!usingResource){
								resourceUsedTime = time;
							}
							resourceUsedTimer += getResourceUseTime(playerInventory->itemInHand.item.type);
						}
					}
					playerInventory->update();
				}
			}
			else if(canControl && closestBox != nullptr){
				if(closestBox->menu->hidden){
					closestBox->menu->hidden = false;
					si::playRandomSoundV(nullptr, "box_open", gc::boxVolume);
					closestBox->update();
				}
				else{
					ItemStack& pi = playerInventory->at(playerInventory->selectedSlot);
					ItemStack& ri = closestBox->at(closestBox->selectedSlot);
					if(pi.amount > 0 || ri.amount > 0){
						si::playRandomSoundV(nullptr, "bag", gc::bagVolume);
					}
					if(pi.amount > 0){
						if(ri.amount > 0){
							if(pi.item.type == ri.item.type){
								playerInventory->put(ri, playerInventory->selectedSlot);
							}
							else{
								playerInventory->put(ri);
							}
						}
						else{
							closestBox->swap(pi, closestBox->selectedSlot);
						}
					}
					else{
						closestBox->swap(pi, closestBox->selectedSlot);
					}
					playerInventory->update();
					closestBox->update();
				}
			}
			else if(canControl && nearTruck_){
				completeState = IN_TRUCK;
			}
		}

		if(!controller->usingController){
			if(playerInventory->itemInHand.amount > 0){
				cursorSet->hand();
			}
			else if(closestBox != nullptr && closestBox->menu->hidden && closestBox->getSprite(world->time())->sprite()->getGlobalBounds().contains(
				float(manager->inputManager->mouseX()),
				float(manager->inputManager->mouseY())
				)){
				cursorSet->box();
			}
			else if(nearTruck_ && truck->getSprite(world->time())->sprite()->getGlobalBounds().contains(
				float(manager->inputManager->mouseX()),
				float(manager->inputManager->mouseY())
				)){
				cursorSet->truck();
			}
		}

		for(size_t i = 0; i < resources.size(); i++){
			if(player->bounds(world->time()).intersects(resources[i].first->bounds(world->time()))){
				si::playRandomSoundV(nullptr, "bag", gc::bagVolume);
				playerInventory->put(resources[i].second);
				playerInventory->update();
				if(resources[i].second.amount == 0){
					resources[i].first->kill();
					resources.erase(resources.begin() + i--);
				}
			}
		}

		bool journalOpenSound = target == dist;
		target = closest != nullptr && handBook->isClosed() ? 0.0f : dist;
		if(journalOpenSound && target == 0.0f){
			si::playSound(nullptr, "interface.journal_open");
		}

		gi::camera(world->dt());
		world->render();

		if(resourceUsedTime.asMilliseconds() != 0 && resourceUseProgress < 1.0f){
			CoreSprite* cs = player->getSprite(time);
			resourceUseBar->position = Vector(gi::TARGET_WIDTH / 2.0f - 50, gi::TARGET_HEIGHT / 2.0f - cs->h() * player->scale / 2.0f);
			gi::draw(resourceUseBar, time);
		}

		/*
		Journal
		*/
		if(completeState == IN_GAME){
			Vector cv = Vector(0.0f, target - actual);
			if(cv.length() > 0.0f){
				cv *= world->dt() * (d - (d / cv.length()));
				actual += cv.y;
			}
			journal->position.x = 50;
			journal->position.y = -10 - actual;
			gi::draw(*journal->getSprite(world->time()));
			if(closest != nullptr){
				closest->hasSeenJournal = true;
				gi::draw(closest->customJournal->lines, (journal->position.x + 120) * gi::dxiz(), (journal->position.y + 300) * gi::dyiz(), 530 * gi::dxiz(), 720 * gi::dyiz());

				if(closest->hasTimer()){
					gi::draw(
						*skull,
						journal->position.x + 330,
						journal->position.y + 400,
						45,
						60
						);
					closest->deathBar.position = Vector((journal->position.x + 380), (journal->position.y + 410));
					gi::draw(&closest->deathBar, world->time());
				}
			}
		}
		//

		if(completeState == IN_GAME && timer.asMilliseconds() > 0){
			gi::draw(*timerHud->getSprite(world->time()));
			sf::Text tt;
			tt.setPosition((gi::TARGET_WIDTH - 200.0f) * gi::dxiz(), 11.0f * gi::dyiz());
			tt.setFont(gi::textFont);
			tt.setCharacterSize(64);
			tt.setString(std::to_string(int((timer - time).asSeconds())));
			tt.scale(1.0f * gi::dxiz(), 1.0f * gi::dyiz());
			gi::renderWindow->draw(tt);

			float prog = time / timer;
			if(prog >= gc::armyMarchStart){
				if(armyMarchId == 0){
					armyMarchId = si::playSoundV(nullptr, "army.march", 0.0f, true);
				}
				else{
					float av = gc::armyMarchVolume * (prog - gc::armyMarchStart) * (1.0f / gc::armyMarchStart);
					si::setSoundV(armyMarchId, av * av);
				}
			}
			if((timer - time).asSeconds() <= gc::armyScreamLength){
				if(armyScreamId == 0){
					armyScreamId = si::playSoundV(nullptr, "army.scream", gc::armyScreamVolume);
					nextWarSound = time;
				}
			}
		}

		handBook->render();

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
}

bool Level::done(){
	bool d = completeState != IN_GAME && fadeValue >= 1.0f;
	if(d){
		totalCivil = 0;
		totalSoldier = 0;
		totalGeneral = 0;
		savedCivil = 0;
		savedSoldier = 0;
		savedGeneral = 0;

		for(Injured* i : injured){
			switch(i->type){
			case CIVIL:
				totalCivil++;
				if(i->survived()){
					savedCivil++;
				}
				break;
			case SOLDIER:
				totalSoldier++;
				if(i->survived()){
					savedSoldier++;
				}
				break;
			case GENERAL:
				totalGeneral++;
				if(i->survived()){
					savedGeneral++;
				}
				break;
			}
		}
	}
	return d;
}

void Level::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(completeState != IN_GAME || world->isPaused()){
		return;
	}
	if(event.pressed() && event.button() == sf::Mouse::Left){
		if(canUseResource && closest != nullptr && playerInventory->itemInHand.amount > 0){
			if(closest->isAlive()){
				sf::FloatRect ifr = closest->bounds(world->time());
				if(ifr.contains(gi::wx(float(manager->inputManager->mouseX())), gi::wy(float(manager->inputManager->mouseY())))){
					if(!hasUsedResource){
						hasUsedResource = playerInventory->itemInHand.amount > 0;
					}
					if(playerInventory->itemInHand.amount > 0){
						if(closest->use(playerInventory->itemInHand)){
							resourceUseInjured = closest;
							if(gc::useTimerReset){
								resourceUsedTime = time;
								resourceUsedTimer = getResourceUseTime(playerInventory->itemInHand.item.type);
							}
							else{
								if(!usingResource){
									resourceUsedTime = time;
								}
								resourceUsedTimer += getResourceUseTime(playerInventory->itemInHand.item.type);
							}
						}
						playerInventory->update();
					}
				}
			}
		}
		else if(canControl && closestBox != nullptr && closestBox->menu->hidden && closestBox->getSprite(world->time())->sprite()->getGlobalBounds().contains(
			float(event.x()),
			float(event.y())
			)){
			if(closestBox->menu->hidden = !closestBox->menu->hidden){
				si::playRandomSoundV(nullptr, "box_close", gc::boxVolume);
			}
			else{
				si::playRandomSoundV(nullptr, "box_open", gc::boxVolume);
			}
		}
		else if(canControl && nearTruck_ && truck->getSprite(world->time())->sprite()->getGlobalBounds().contains(
			float(event.x()),
			float(event.y())
			)){
			completeState = IN_TRUCK;
		}
	}
}

Injured* Level::nearestInjured(const float& maxDistance){
	Injured* i = nullptr;
	float d = maxDistance;

	sf::FloatRect pfr = player->bounds(world->time());
	Vector cp((pfr.left + pfr.width / 2.0f), (pfr.top + pfr.height / 2.0f));

	for(Injured* inj : injured){
		if(!inj->isAlive()){
			continue;
		}
		sf::FloatRect ifr = inj->bounds(world->time());
		float distance = math::distance(cp.x, cp.y, ifr.left + ifr.width / 2, ifr.top + ifr.height / 2);
		if(d > distance){
			Vector v(
				(ifr.left + ifr.width / 2.0f) - cp.x,
				(ifr.top + ifr.height / 2.0f) - cp.y
				);
			float l = v.length();
			v = v.norm();
			bool passed = true;

			for(float f = stepDistance; f < l && passed; f += stepDistance){
				sf::Vector2f fv = (cp + (v * f)).v2f();
				size_t min = world->binarySearchRenderOffset(cp.y - 1000.0f, LAYER2);
				size_t max = world->binarySearchRenderOffset(cp.y + 1000.0f, LAYER2);

				for(size_t di1 = min; di1 < world->drawables[LAYER2].size() && di1 <= max; di1++){
					drawable::Drawable* d = world->drawables[LAYER2][di1];

					if(d->hideUnderCamera || d == inj || d == player || (d->reference.length() > 9 && d->reference.substr(0, 9) == "Props.Bed") || (d->reference.length() > 9 && d->reference.substr(0, 7) == "Decals.")){
						continue;
					}
					if(math::interv(fv.x, d->position.x) + math::interv(fv.y, d->position.y) > MAX_COLLISION_DISTANCE){
						continue;
					}

					if(d->bounds(world->time()).contains(fv)){
						passed = false;
						break;
					}
				}
			}

			if(passed){
				d = distance;
				i = inj;
			}
		}
	}
	return i;
}

ResourceBox* Level::nearestResourceBox(const float& maxDistance){
	ResourceBox* i = nullptr;
	float d = maxDistance;

	sf::FloatRect pfr = player->bounds(world->time());
	Vector cp((pfr.left + pfr.width / 2.0f), (pfr.top + pfr.height / 2.0f));

	for(ResourceBox* inj : resourceBoxes){
		if(!inj->isAlive()){
			continue;
		}
		sf::FloatRect ifr = inj->bounds(world->time());
		float distance = math::distance(cp.x, cp.y, ifr.left + ifr.width / 2, ifr.top + ifr.height / 2);
		if(d > distance){
			Vector v(
				(ifr.left + ifr.width / 2.0f) - cp.x,
				(ifr.top + ifr.height / 2.0f) - cp.y
				);
			float l = v.length();
			v = v.norm();
			bool passed = true;

			for(float f = stepDistance; f < l && passed; f += stepDistance){
				sf::Vector2f fv = (cp + (v * f)).v2f();
				size_t min = world->binarySearchRenderOffset(cp.y - 1000.0f, LAYER2);
				size_t max = world->binarySearchRenderOffset(cp.y + 1000.0f, LAYER2);

				for(size_t di1 = min; di1 < world->drawables[LAYER2].size() && di1 <= max; di1++){
					drawable::Drawable* d = world->drawables[LAYER2][di1];

					if(d->hideUnderCamera || d == inj || d == player || (d->reference.length() > 9 && d->reference.substr(0, 7) == "Decals.")){
						continue;
					}
					if(math::interv(fv.x, d->position.x) + math::interv(fv.y, d->position.y) > MAX_COLLISION_DISTANCE){
						continue;
					}

					if(d->bounds(world->time()).contains(fv)){
						passed = false;
						break;
					}
				}
			}

			if(passed){
				d = distance;
				i = inj;
			}
		}
	}
	return i;
}

bool Level::nearTruck(const float& maxDistance){
	sf::FloatRect pfr = player->bounds(world->time());
	sf::FloatRect ifr = truck->bounds(world->time());
	return maxDistance >= math::distance(pfr.left + pfr.width / 2, pfr.top + pfr.height / 2, ifr.left + ifr.width / 2, ifr.top + ifr.height / 2);
}