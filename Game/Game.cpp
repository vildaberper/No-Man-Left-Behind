#include "Game.h"

#include <string>
#include <vector>
#include <iostream>

#include "World.h"
#include "Cursor.h"
#include "Configuration.h"

using namespace sf;
using namespace std;
using namespace math;

Game::Game(){
	menuCommand = new std::string();
}


Game::~Game(){
	delete menuCommand;
}

void Game::on(KeyboardEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(event.pressed() && gc::developerMode){
		switch(event.key()){
		case Keyboard::C:
			if(event.first()){
				gi::collisionBoxes = !gi::collisionBoxes;
			}
			break;
		case Keyboard::L:
			if(event.first()){
				gi::logAlwaysActive = !gi::logAlwaysActive;
			}
			break;
		}
	}
}

void Game::on(MouseMoveEvent& event){
	if(clock.getElapsedTime() < ignoreMouseMoveUntil){
		return;
	}
	if((clock.getElapsedTime() - lastMouseMove).asMilliseconds() < 100){
		controller->usingController = false;
	}
	lastMouseMove = clock.getElapsedTime();
}

void Game::on(MouseButtonEvent& event){
	if(controller->usingController){
		*menuCommand = "";
	}
	controller->usingController = false;
}

void Game::run(){
	clock.restart();
	gi::showCursor = false;
	c::initialize();
	gc::initialize();
	gi::initalize();
	gi::smoothCamera = true;

	if(gc::developerMode){
		c::showLog = true;
		gi::setUsingConsole(true);
	}

	random::initialize();

	Time lastTime;
	while(gi::startOfFrame()){
		if(!managerInitialized){
			gi::darken(1.0f);
			gi::endOfFrame();
			manager = new Manager();
			manager->initialize(gi::renderWindow);
			controller = new Controller();
			controller->initialize(manager);
			managerInitialized = true;
			inputListenerId = manager->inputManager->registerListener(this);
			jmanager = new JournalManager();

			playerInventory = new PlayerInventory(controller, manager, gc::inventorySize);
			handBook = new Handbook(manager);

			mainmenu = manager->spriteManager->getSprite("menu.bg");
			a = 1.0f;
			at = 0.0f;

			creditbg = manager->spriteManager->getSprite("bg.intro");
			nmlbb = manager->spriteManager->getSprite("logo.nmlb-bag");
			morsolw = manager->spriteManager->getSprite("logo.moronic_solutionsw");

			gi::inputManager = manager->inputManager;

			transition = new Transition(manager);

			/*
				Menu
			*/
			mainMenu = new Menu();
			mainMenu->position = Vector(90.0f, gi::TARGET_HEIGHT - 50 - 62 * 2 * 4);
			mainMenu->size = Vector(192 * 2, 62 * 2 * 4);
			mainMenu->hidden = false;
			mainMenu->type = VERTICAL;

			pauseMenu = new Menu();
			pauseMenu->position = Vector(90.0f, gi::TARGET_HEIGHT - 50 - 62 * 2 * 5);
			pauseMenu->size = Vector(192 * 2, 62 * 2 * 5);
			pauseMenu->hidden = true;
			pauseMenu->type = VERTICAL;

			optionsMenu = new Menu();
			optionsMenu->position = Vector(90.0f, gi::TARGET_HEIGHT - 50 - 62 * 2 * 4);
			optionsMenu->size = Vector(192 * 2, 62 * 2 * 4);
			optionsMenu->hidden = true;
			optionsMenu->type = VERTICAL;

			// Options
			checked = manager->spriteManager->getSprite("menu.checked");
			unchecked = manager->spriteManager->getSprite("menu.unchecked");

			itemFullscreen = new MenuItem();
			itemFullscreen->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.fullscreen"), NULL);
			itemFullscreen->type = TEXTURE;
			itemFullscreen->selectedPrefix = "options.fullscreen";
			itemFullscreen->selectedString = menuCommand;
			itemFullscreen->darkenOnMouseOver = true;
			optionsMenu->items.push_back(itemFullscreen);

			itemVsync = new MenuItem();
			itemVsync->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.vsync"), NULL);
			itemVsync->type = TEXTURE;
			itemVsync->selectedPrefix = "options.vsync";
			itemVsync->selectedString = menuCommand;
			itemVsync->darkenOnMouseOver = true;
			optionsMenu->items.push_back(itemVsync);

			MenuItem* mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.apply"), NULL);
			mi->selectedPrefix = "options.apply";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			optionsMenu->items.push_back(mi);

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.back"), NULL);
			mi->selectedPrefix = "options.back";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			optionsMenu->items.push_back(mi);

			//

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.continue"), NULL);
			mi->selectedPrefix = "continue";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			pauseMenu->items.push_back(mi);

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.new game"), NULL);
			mi->selectedPrefix = "new game";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			mainMenu->items.push_back(mi);
			pauseMenu->items.push_back(mi);

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.options"), NULL);
			mi->selectedPrefix = "options";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			mainMenu->items.push_back(mi);
			pauseMenu->items.push_back(mi);

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.credits"), NULL);
			mi->selectedPrefix = "credits";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			mainMenu->items.push_back(mi);
			pauseMenu->items.push_back(mi);

			mi = new MenuItem();
			mi->background = new TexBar(NULL, manager->spriteManager->getTexture("menu.quit"), NULL);
			mi->selectedPrefix = "quit";
			mi->selectedString = menuCommand;
			mi->darkenOnMouseOver = true;
			mainMenu->items.push_back(mi);
			pauseMenu->items.push_back(mi);

			manager->menuManager->menus["mainmenu"] = mainMenu;
			manager->menuManager->menus["pausemenu"] = pauseMenu;
			manager->menuManager->menus["optionsmenu"] = optionsMenu;
			//

			std::vector<std::string>* lines = c::baseDir.child("credits.txt").readTextFile();
			for(size_t i = 0; i < lines->size(); i++){
				credits.push_back((*lines)[i]);
			}
			delete lines;

			cursorSet = new CursorSet(manager);
			cursorSet->main();

			manager->soundManager->minDistance = gc::hearingDistance;

			clock.restart();
			continue;
		}

		Time time = clock.getElapsedTime();
		float dt = (time - lastTime).asSeconds();

		manager->tick(gi::renderWindow, time, (level == NULL || level->world == NULL) ? dt : level->world->dt());

		if(a > at && a > 0.0f){
			a -= dt * as;
		}
		else if(a < at && a < 1.0f){
			a += dt * as;
		}
		if(a < 0.0f){
			a = 0.0f;
		}
		else if(a > 1.0f){
			a = 1.0f;
		}

		switch(state){
		case MAIN_MENU:
		{
			if(firstStateFrame){
				firstStateFrame = false;
				menuIndex = 0;
				if(lastState == OPTIONS){
					as = 5.0f;
					menuIndex = 1;
				}
				else if(lastState == CREDITS){
					as = 5.0f;
					menuIndex = 2;
				}
			}

			lastState = MAIN_MENU;
			cursorSet->main();
			gi::showCursor = !controller->usingController;
			mainMenu->hidden = false;
			handBook->openMenu->hidden = true;
			playerInventory->menu->hidden = true;
			if(!playingMenuMusic && shouldPlayMenuMusic){
				introId = si::playMusic("menu.intro", true, true, false);
				mainId = si::queueMusic(introId, "menu.main", false, true, true);
				playingMenuMusic = true;
			}

			if(controller->isPressed(MENUUP)){
				menuIndex--;
			}
			if(controller->isPressed(MENUDOWN)){
				menuIndex++;
			}
			menuIndex = math::range(menuIndex, mainMenu->items.size() - 1);
			for(size_t i = 0; i < mainMenu->items.size(); i++){
				mainMenu->items[i]->darkenOnMouseOver = !controller->usingController;
				mainMenu->items[i]->highlight = controller->usingController && i == menuIndex;
			}
			bool wuc = controller->usingController;
			if(controller->isPressed(INTERACT) && wuc){
				*menuCommand = mainMenu->items[menuIndex]->selectedPrefix;
			}
			gi::background(*mainmenu);
			if(menuCommand->length() > 0){
				as = 2.0f;
				if(*menuCommand == "new game"){
					newGame();
					at = 1.0f;
					nextState = TRANSITION;
				}
				else if(*menuCommand == "options"){
					at = 1.0f;
					as = 5.0f;
					nextState = OPTIONS;
				}
				else if(*menuCommand == "credits"){
					at = 1.0f;
					nextState = CREDITS;
				}
				else if(*menuCommand == "quit"){
					at = 1.0f;
					nextState = CLOSE;
				}
				*menuCommand = "";
			}
			if(a >= 1.0f && at == 1.0f){
				state = nextState;
				currentLevel = 0;
				at = 0.0f;
				as = 1.0f;
				mainMenu->hidden = true;
				firstStateFrame = true;
			}
			manager->menuManager->draw(time);
			break;
		}
		case LEVEL_MENU:
		{
			if(firstStateFrame){
				firstStateFrame = false;
				menuIndex = 0;
				if(lastState == OPTIONS){
					as = 5.0f;
					menuIndex = 2;
				}
				else if(lastState == CREDITS){
					as = 5.0f;
					menuIndex = 3;
				}
			}

			lastState = LEVEL_MENU;
			cursorSet->main();
			gi::showCursor = !controller->usingController;
			pauseMenu->hidden = false;
			handBook->openMenu->hidden = true;
			playerInventory->menu->hidden = true;

			if(controller->isPressed(MENUUP)){
				menuIndex--;
			}
			if(controller->isPressed(MENUDOWN)){
				menuIndex++;
			}
			menuIndex = math::range(menuIndex, pauseMenu->items.size() - 1);
			for(size_t i = 0; i < pauseMenu->items.size(); i++){
				pauseMenu->items[i]->darkenOnMouseOver = !controller->usingController;
				pauseMenu->items[i]->highlight = controller->usingController && i == menuIndex;
			}
			bool wuc = controller->usingController;
			if(controller->isPressed(INTERACT) && wuc){
				*menuCommand = pauseMenu->items[menuIndex]->selectedPrefix;
			}
			gi::background(*mainmenu);
			bool esc = controller->isPressed(Command::PAUSE) || controller->isPressed(Command::BACK);
			if(esc || menuCommand->length() > 0){
				as = 10.0f;
				if(esc || *menuCommand == "continue"){
					at = 1.0f;
					nextState = LEVEL;
					level->world->setPaused(false);
				}
				else if(*menuCommand == "new game"){
					newGame();
					at = 1.0f;
					nextState = TRANSITION;
				}
				else if(*menuCommand == "options"){
					at = 1.0f;
					as = 5.0f;
					nextState = OPTIONS;
				}
				else if(*menuCommand == "credits"){
					at = 1.0f;
					nextState = CREDITS;
				}
				else if(*menuCommand == "quit"){
					at = 1.0f;
					nextState = CLOSE;
				}
				*menuCommand = "";
			}
			if(a >= 1.0f && at == 1.0f){
				state = nextState;
				at = 0.0f;
				as = 1.0f;
				pauseMenu->hidden = true;
				firstStateFrame = true;
			}
			manager->menuManager->draw(time);
			break;
		}
		case LEVEL:
			cursorSet->main();
			gi::showCursor = !controller->usingController;
			playerInventory->menu->hidden = false;
			handBook->openMenu->hidden = false;
			shouldPlayMenuMusic = false;
			level->tick();

			if(level->world->isPaused()){
				handBook->forceClose();
				nextState = LEVEL_MENU;
				as = 10.0f;
				at = 1.0f;
				if(a >= 1.0f){
					as = 1.0f;
					state = nextState;
					break;
				}
			}

			if(level->done()){
				if(!(restartLevel = level->completeState == TIME_RAN_OUT)){
					totalCivil += level->totalCivil;
					totalSoldier += level->totalSoldier;
					totalGeneral += level->totalGeneral;
					savedCivil += level->savedCivil;
					savedSoldier += level->savedSoldier;
					savedGeneral += level->savedGeneral;
					currentLevel++;

					timeBonus = level->savedSoldier * gc::timeBonusFromSoldiers;

					float totalS = 0;
					float savedS = 0;
					totalS += totalCivil * gc::stressCivil;
					totalS += totalSoldier * gc::stressSoldier;
					totalS += totalGeneral * gc::stressGeneral;
					savedS += savedCivil * gc::stressCivil;
					savedS += savedSoldier * gc::stressSoldier;
					savedS += savedGeneral * gc::stressGeneral;
					stress = savedS / totalS;

					lastItemInHand = playerInventory->itemInHand;
					if(lastContent != NULL){
						delete[] lastContent;
					}
					lastContent = new ItemStack[playerInventory->getSize()];
					for(size_t i = 0; i < playerInventory->getSize(); i++){
						lastContent[i] = ItemStack(playerInventory->content[i].item, playerInventory->content[i].amount);
					}
					lastExtraResources = level->savedGeneral * gc::resourcesFromGenerals;

					transition->addPage(level, timeBonus, stress, lastExtraResources, std::to_string(currentLevel) + (stress >= gc::stressForA ? "a" : (stress >= gc::stressForB ? "b" : "c")));

					if(currentLevel >= gc::levelProgression.size()){
						transition->end(stress);
					}
				}
				else{
					if(lastContent != NULL){
						playerInventory->itemInHand = lastItemInHand;
						for(size_t i = 0; i < playerInventory->getSize(); i++){
							playerInventory->content[i] = ItemStack(lastContent[i].item, lastContent[i].amount);
						}
					}
					transition->newRestart();
				}

				handBook->forceClose();
				state = TRANSITION;
				level->world->cleanAll(true);
				firstStateFrame = true;
				at = 0.0f;
				a = 1.0f;
			}
			break;
		case TRANSITION:
		{
			if(firstStateFrame){
				firstStateFrame = false;
				transtionDone = false;
				cursorSet->main();
				handBook->openMenu->hidden = true;
				playerInventory->menu->hidden = true;
				transition->setHidden(restartLevel);
			}
			gi::showCursor = !controller->usingController;
			if(
				(!restartLevel && transition->hidden) || controller->isPressed(Command::SKIP)){
				at = 1.0f;
				transtionDone = true;
				transition->setHidden(true);
			}
			if(restartLevel){
				transition->renderRestart();
			}
			else{
				if(controller->isPressed(Command::LB)){
					transition->turnLeft();
				}
				if(controller->isPressed(Command::RB)){
					transition->turnRight();
				}
				transition->render();
			}
			manager->menuManager->draw(time);
			if(transtionDone && a >= 1.0f){
				if(playingMenuMusic){
					si::stopMusic(introId);
					si::stopMusic(mainId);
					playingMenuMusic = false;
				}
				if(currentLevel >= gc::levelProgression.size()){
					delete level;
					level = NULL;
					state = COMPLETE;
					continue;
				}
				else{
					if(level != NULL){
						delete level;
					}
					gi::darken(1.0f);
					gi::endOfFrame();
					level = new Level(manager, controller, jmanager, cursorSet);
					level->extraResources = lastExtraResources;
					level->playerInventory = playerInventory;
					level->handBook = handBook;
					level->load(gc::levelDir.child(gc::levelProgression[currentLevel] + ".txt"));

					level->begin(stress, timeBonus);
					clock.restart();
					state = LEVEL;
					at = 0.0f;
					transition->setHidden(true);
				}
				firstStateFrame = true;
			}
			break;
		}
		case OPTIONS:
		{
			gi::showCursor = !controller->usingController;
			if(firstStateFrame){
				firstStateFrame = false;
				c::initialize();
				if(gc::developerMode){
					c::showLog = true;
				}
				itemFullscreen->sprite = c::fullscreen ? checked : unchecked;
				itemVsync->sprite = c::verticalSync ? checked : unchecked;
				changedOptions = false;
				menuIndex = 0;
			}
			as = 5.0f;
			optionsMenu->hidden = false;

			if(controller->isPressed(MENUUP)){
				menuIndex--;
			}
			if(controller->isPressed(MENUDOWN)){
				menuIndex++;
			}
			menuIndex = math::range(menuIndex, optionsMenu->items.size() - 1);
			for(size_t i = 0; i < optionsMenu->items.size(); i++){
				optionsMenu->items[i]->darkenOnMouseOver = !controller->usingController;
				optionsMenu->items[i]->highlight = controller->usingController && i == menuIndex;
			}
			bool wuc = controller->usingController;
			if(controller->isPressed(INTERACT) && wuc){
				*menuCommand = optionsMenu->items[menuIndex]->selectedPrefix;
			}
			gi::background(*mainmenu);
			bool esc = controller->isPressed(Command::PAUSE) || controller->isPressed(Command::BACK);
			if(esc || menuCommand->length() > 0){
				if(*menuCommand == "options.fullscreen"){
					c::fullscreen = !c::fullscreen;
					itemFullscreen->sprite = c::fullscreen ? checked : unchecked;
					changedOptions = true;
				}
				else if(*menuCommand == "options.vsync"){
					c::verticalSync = !c::verticalSync;
					itemVsync->sprite = c::verticalSync ? checked : unchecked;
					changedOptions = true;
				}
				else if(changedOptions && *menuCommand == "options.apply"){
					changedOptions = false;
					gi::renderWindow->close();
					gi::initalize();

					c::save();
					menuIndex = 3;
					ignoreMouseMoveUntil = clock.getElapsedTime() + sf::milliseconds(500);
				}
				else if(esc || *menuCommand == "options.back"){
					at = 1.0f;
				}
				*menuCommand = "";
			}
			if(a >= 1.0f && at == 1.0f){
				state = lastState;
				lastState = OPTIONS;
				at = 0.0f;
				as = 1.0f;
				optionsMenu->hidden = true;
				firstStateFrame = true;
			}
			manager->menuManager->draw(time);
			break;
		}
		case CREDITS:
		{
			if(creditTime.asMilliseconds() == 0){
				creditTime = time;
				creditLogoTime = sf::milliseconds(0);
				cursorSet->hide();
			}
			sf::Text t;
			t.setFont(gi::menuFont);
			t.setCharacterSize(int(50 * gi::dxiz()));
			gi::background(*creditbg);
			float lastY;
			for(size_t i = 0; i < credits.size(); i++){
				t.setString(credits[i]);
				float xp = (gi::TARGET_WIDTH / 2.0f - t.getGlobalBounds().width / 2.0f) * gi::dxiz();
				float yp = (lastY = (gi::TARGET_HEIGHT + 10 + i * 70.0f - (time - creditTime).asSeconds() * 100.0f)) * gi::dyiz();

				t.setColor(sf::Color(0, 0, 0, 255));
				float a = 0.0f;
				float r = (3.0f * gi::dxiz());
				for(unsigned int i = 0; i < 8; i++){
					t.setPosition(xp + r * cos(a), yp + r * sin(a));
					gi::renderWindow->draw(t);
					a += PI / 4;
				}
				t.setColor(sf::Color(255, 255, 255, 255));
				t.setPosition(xp, yp);
				gi::renderWindow->draw(t);
			}
			if(lastY < -50){
				if(creditLogoTime.asMilliseconds() == 0){
					creditLogoTime = time;
				}
				gi::draw(
					*nmlbb,
					gi::TARGET_WIDTH / 2.0f - 404,
					gi::TARGET_HEIGHT / 2.0f - 484,
					808,
					968,
					(time - creditLogoTime).asSeconds() / 3.0f
					);
			}
			gi::draw(
				*morsolw,
				gi::TARGET_WIDTH - 310 - 10,
				gi::TARGET_HEIGHT - 138 - 5,
				310,
				138,
				(time - creditTime).asSeconds() / 3.0f
				);
			if(controller->isPressed(Command::SKIP)){
				at = 1.0f;
				nextState = lastState;
			}
			if(a >= 1.0f && at == 1.0f){
				lastState = CREDITS;
				creditTime = sf::milliseconds(0);
				state = nextState;
				at = 0.0f;
				as = 1.0f;
				gi::showCursor = true;
				firstStateFrame = true;
			}
			break;
		}
		case COMPLETE:
			cursorSet->hide();
			shouldPlayMenuMusic = true;
			a = 1.0f;
			at = 0.0f;
			if(!playingMenuMusic && shouldPlayMenuMusic){
				introId = si::playMusic("menu.intro", true, true, false);
				mainId = si::queueMusic(introId, "menu.main", false, true, true);
				playingMenuMusic = true;
			}
			lastState = MAIN_MENU;
			state = CREDITS;
			firstStateFrame = true;
			break;
		case CLOSE:
			if(firstStateFrame){
				firstStateFrame = false;
				gi::renderWindow->close();
			}
			break;
		}

		if(a > 0.0f){
			gi::darken(a);
		}

		gi::drawLog();

		lastTime = time;
		gi::endOfFrame();
	}
	manager->finalize(gi::renderWindow);
	gi::finalize();
	delete controller;
	delete manager;
	delete jmanager;
}

void Game::newGame(){
	playerInventory->clear();
	playerInventory->itemInHand.amount = 0;

	lastExtraResources = 0;
	lastItemInHand.amount = 0;
	delete[] lastContent;
	lastContent = NULL;

	totalCivil = 0;
	totalSoldier = 0;
	totalGeneral = 0;
	savedCivil = 0;
	savedSoldier = 0;
	savedGeneral = 0;

	timeBonus = 0.0f;

	stress = 1.0f;

	currentLevel = 0;

	handBook->current = 0;

	transition->reset();
	firstStateFrame = true;
}