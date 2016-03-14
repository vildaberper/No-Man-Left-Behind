#include "Level.h"

float stepDistance = 5.0f;

/*

float v = fmod((cl.getElapsedTime().asSeconds() / 2.0f), 1.0f);
if(v < 0.0f){
v = 0.0f;
}
else if(v > 1.0f){
v = 1.0f;
}
float w = 300;
float h = 150;
sf::ConvexShape t;
t.setPosition(500, 500);
t.setFillColor(sf::Color(255, 255, 255, 255));
t.setOutlineColor(sf::Color(0, 0, 0, 255));
t.setOutlineThickness(7);
t.setPointCount(3);
t.setPoint(0, sf::Vector2f(0, 0));
t.setPoint(1, sf::Vector2f(w, 0));
t.setPoint(2, sf::Vector2f(0, h));
renderWindow->draw(t);

t.setFillColor(sf::Color(104, 24, 24, 255));
t.setOutlineColor(sf::Color(0, 0, 0, 255));
t.setOutlineThickness(7);
t.setPointCount(3);
t.setPoint(0, sf::Vector2f(0, h * (1.0f - v)));
t.setPoint(1, sf::Vector2f(w * v, h * (1.0f - v)));
t.setPoint(2, sf::Vector2f(0, h));
renderWindow->draw(t);

*/

Level::Level(Manager* manager, Controller* controller, JournalManager* jmanager){
	Level::manager = manager;
	Level::controller = controller;
	Level::jmanager = jmanager;
}

Level::~Level(){
	manager->inputManager->unregisterListener(listenerId);
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
	truckspawn = c.floatVector("general.truckspawn");
	useTruck = c.boolValue("general.useTruck");
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
			size_t type = random::random(j->injured.size() - 1);

			Injured* in = new Injured();
			in->initialize(manager, j->injured[type], j);
			in->position = c.floatVector("injured." + id + ".position");
			injured.push_back(in);
		}
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
	timerHud = new Animatable();
	timerHud->setAnimationType(STATES);
	timerHud->applyAnimation(manager, "timer");
	timerHud->scale = 0.25f;
	timerHud->position = Vector(gi::TARGET_WIDTH - timerHud->getSprite(sf::milliseconds(0))->w() * timerHud->scale, 0.0);
	timerHud->viewRelative = true;

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

	for(Injured* inj : injured){
		world->addDrawable(inj, LAYER2);
	}

	if(player == NULL){
		player = new Player();
		player->initialize(manager);
		CoreSprite* cs = player->getSprite(sf::milliseconds(0));
		gi::relativeOffset.y = player->scale * (-(cs->h() / 2.0f) + cs->h() * player->cb.offset.y + cs->h() * player->cb.size.y / 2.0f);
	}

	truck = new Truck();
	truck->initialize(manager);
	if(useTruck){
		truck->position = spawn;
		truck->position.y -= truck->getSprite(world->time())->h() * truck->scale * truck->cb.renderOffset / gi::dy();
		manager->soundManager->relative = truck;
	}
	else{
		truck->speed = 0.0f;
		truck->targetspeed = 0.0f;
		truck->position = truckspawn;
		truck->position.y -= truck->getSprite(world->time())->h() * truck->scale * truck->cb.renderOffset / gi::dy();

		player->position = spawn;
		player->position.y -= player->getSprite(world->time())->h() * player->scale * player->cb.renderOffset / gi::dy();
		world->addDrawable(player, LAYER2);
		playerInventory->menu->hidden = false;
		manager->soundManager->relative = player;
	}
	world->addDrawable(truck, LAYER2);

	if(!useTruck){
		state = PLAYING;
		clock.restart();
	}

	playerInventory->update();

	journal = new Animatable();
	journal->setAnimationType(STATES);
	journal->applyAnimation(manager, "journal");
	journal->position = Vector(0.0, 0.0);
	journal->scale = 0.85f;
	journal->viewRelative = true;

	if(musicIntro.length() > 0){
		introId = si::playMusic(musicIntro, true, false, false);
		mainId = si::queueMusic(introId, musicMain, false, true, true);
	}
	else if(musicMain.length() > 0){
		mainId = si::playMusic(musicMain, true, true, true);
	}

	for(drawable::Drawable* d : world->drawables[LAYER2]){
		if(
			(d->reference.size() > 10 && d->reference.substr(0, 10) == "Props.Box ")
			|| (d->reference.size() > 18 && d->reference.substr(0, 18) == "Props.Medical Box ")
			|| (d->reference.size() > 22 && d->reference.substr(0, 22) == "Props.Medical Cabinet ")
			){
			logger::debug("Found resource box: " + d->reference);
			resourceBoxes.push_back(new ResourceBox(controller, manager, 7, playerInventory, d));
		}
	}

	for(Injured* inj : injured){
		std::vector<ItemStack> iss;
		for(Resource r : inj->journal->requirements){
			iss.push_back(ItemStack(r, 1));
		}

		bool empty = iss.size() == 0;
		while(!empty){
			for(ItemStack& is : iss){
				if(is.amount > 0){
					size_t rb = random::random(resourceBoxes.size() - 1);
					resourceBoxes[rb]->put(is);
				}
			}

			empty = true;
			for(ItemStack is : iss){
				empty = is.amount > 0 ? false : empty;
			}
		};
	}

	for(ResourceBox* rb : resourceBoxes){
		rb->update();
		world->addDrawable(rb, LAYER2);
	}

	listenerId = manager->inputManager->registerListener(this);
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

	if(manager->inputManager->isFirstPressed(sf::Keyboard::O)){
		completeState = IN_TRUCK;
	}

	if(fadeValue > 0.0f && completeState == IN_GAME){
		fadeValue -= world->dt() / 2.0f;
	}
	if(fadeValue < 1.0f && completeState != IN_GAME){
		fadeValue += world->dt() / 2.0f;
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
			clock.restart();
			manager->soundManager->relative = player;
		}
		break;
	}
	case PLAYING:
	{
		if(clock.getElapsedTime() > sf::milliseconds(10000)){
			nearTruck_ = nearTruck(250.0f);
			truck->highlight = nearTruck_;
		}
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

		ResourceBox* cb = nearestResourceBox(150.0f);
		if(cb != NULL){
			if(closestBox != NULL){
				if(cb != closestBox){
					closestBox->highlight = false;
					closestBox->menu->hidden = true;
				}
			}
			closestBox = cb;
			closestBox->highlight = true;
		}
		else{
			if(closestBox != NULL){
				closestBox->highlight = false;
				closestBox->menu->hidden = true;
				closestBox = cb;
			}
		}

		if(handBook->isOpen()){
			if(controller->isPressed(LB)){
				handBook->turnLeft();
			}
			if(controller->isPressed(RB)){
				handBook->turnRight();
			}
		}

		if(controller->isPressed(LB)){
			playerInventory->selectedSlot--;
			playerInventory->update();
		}
		if(controller->isPressed(RB)){
			playerInventory->selectedSlot++;
			playerInventory->update();
		}

		if(controller->isPressed(INTERACT)){
			if(closest != NULL){
				closest->use(playerInventory->selectedItem());
				playerInventory->update();
			}
			else if(closestBox != NULL){
				closestBox->menu->hidden = !closestBox->menu->hidden;
			}
			else if(nearTruck_){
				completeState = IN_TRUCK;
			}
		}
		bool journalOpenSound = target == dist;
		target = closest != NULL ? 0.0f : dist;
		if(journalOpenSound && target == 0.0f){
			si::playSound(NULL, "interface.journal_open");
		}

		gi::camera(world->dt());
		world->render();

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
			if(closest != NULL){
				gi::draw(closest->customJournal->lines, (journal->position.x + 120) * gi::dxiz(), (journal->position.y + 300) * gi::dyiz(), 530 * gi::dxiz(), 720 * gi::dyiz());
			}
		}
		//
		handBook->render();

		manager->menuManager->draw(world->time());

		playerInventory->render();

		if(timer.asMilliseconds() > 0){
			gi::draw(*timerHud->getSprite(world->time()));
			sf::Text tt;
			tt.setPosition((gi::TARGET_WIDTH - 100.0f) * gi::dxiz(), 5.5f * gi::dyiz());
			tt.setFont(gi::textFont);
			tt.setString(std::to_string(int((timer - clock.getElapsedTime()).asSeconds())));
			tt.scale(1.0f * gi::dxiz(), 1.0f * gi::dyiz());
			gi::renderWindow->draw(tt);
		}

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
	if(completeState == IN_GAME && timer.asMilliseconds() > 0 && clock.getElapsedTime() > timer){
		completeState = TIME_RAN_OUT;
	}
	return completeState != IN_GAME && fadeValue >= 1.0f;
}

void Level::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(event.pressed() && event.button() == sf::Mouse::Left){
		if(closest != NULL && playerInventory->itemInHand.amount > 0){
			if(closest->isAlive()){
				sf::FloatRect ifr = closest->bounds(world->time());
				if(ifr.contains(gi::wx(float(manager->inputManager->mouseX())), gi::wy(float(manager->inputManager->mouseY())))){
					closest->use(playerInventory->itemInHand);
					playerInventory->update();
				}
			}
		}
	}
}

Injured* Level::nearestInjured(const float& maxDistance){
	Injured* i = NULL;
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

						if(d == inj || d == player || (d->reference.length() > 9 && d->reference.substr(0, 9) == "Props.Bed")){
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
	ResourceBox* i = NULL;
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

					if(d == inj || d == player){
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