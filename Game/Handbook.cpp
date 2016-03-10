#include "Handbook.h"

using namespace handbook;

Handbook::Handbook(Manager* m){
	manager = m;
	hc = new std::string();
	setAnimationType(STATES, 4);
	applyAnimation(m, "handbook");
	viewRelative = true;
	currentAnimation = nextAnimation = CLOSED;
	position.x = gi::TARGET_WIDTH / 2.0f;
	position.y = gi::TARGET_HEIGHT / 2.0f;
	scale = 1.4f;

	CoreSprite* cs = getSprite(sf::microseconds(0));
	position.x -= cs->w() * scale / 2.0f;
	position.y -= cs->h() * scale / 2.0f;

	std::vector<std::string>* lines = gc::handBookFile.readTextFile();
	Page* p = new Page();

	for(size_t i = 0; i < lines->size(); i++){
		std::string line = (*lines)[i];

		if(line == "%%"){
			pages.push_back(p);
			p = new Page();
			continue;
		}

		p->lines.push_back(line);
	}
	pages.push_back(p);

	delete lines;

	openMenu = new Menu();
	openMenu->position = Vector(gi::TARGET_WIDTH - 300.0f, gi::TARGET_HEIGHT - 100.0f);
	openMenu->size = Vector(100.0f, 100.0f);
	openMenu->hidden = false;
	openMenu->type = HORIZONTAL;
	openMenu->background = new TexBar(
		manager->spriteManager->getTexture("book.left"),
		manager->spriteManager->getTexture("book.middle"),
		manager->spriteManager->getTexture("book.right")
		);
	openMenu->drawElementBackgrounds = false;
	manager->menuManager->menus["handbookopen"] = openMenu;
	MenuItem* openMenuItem = new MenuItem();
	openMenuItem->selectedPrefix = "handbookopen";
	openMenuItem->selectedString = hc;
	openMenu->items.push_back(openMenuItem);

	closeMenu = new Menu();
	closeMenu->position = Vector(1580.0f, 100.0f);
	closeMenu->size = Vector(50.0f, 50.0f);
	closeMenu->hidden = true;
	closeMenu->type = HORIZONTAL;
	closeMenu->background = new TexBar(
		NULL,
		manager->spriteManager->getTexture("book.close"),
		NULL
		);
	closeMenu->drawElementBackgrounds = false;
	manager->menuManager->menus["handbookclose"] = closeMenu;
	MenuItem* closeMenuItem = new MenuItem();
	closeMenuItem->selectedPrefix = "handbookclose";
	closeMenuItem->selectedString = hc;
	closeMenu->items.push_back(closeMenuItem);

	leftMenu = new Menu();
	leftMenu->position = Vector(290.0f, 820.0f);
	leftMenu->size = Vector(50.0f, 50.0f);
	leftMenu->hidden = true;
	leftMenu->type = HORIZONTAL;
	leftMenu->background = new TexBar(
		NULL,
		manager->spriteManager->getTexture("book.turnleft"),
		NULL
		);
	leftMenu->drawElementBackgrounds = false;
	manager->menuManager->menus["handbookleft"] = leftMenu;
	MenuItem* leftMenuItem = new MenuItem();
	leftMenuItem->selectedPrefix = "handbookleft";
	leftMenuItem->selectedString = hc;
	leftMenu->items.push_back(leftMenuItem);

	rightMenu = new Menu();
	rightMenu->position = Vector(1580.0f, 820.0f);
	rightMenu->size = Vector(50.0f, 50.0f);
	rightMenu->hidden = true;
	rightMenu->type = HORIZONTAL;
	rightMenu->background = new TexBar(
		NULL,
		manager->spriteManager->getTexture("book.turnright"),
		NULL
		);
	rightMenu->drawElementBackgrounds = false;
	manager->menuManager->menus["handbookright"] = rightMenu;
	MenuItem* rightMenuItem = new MenuItem();
	rightMenuItem->selectedPrefix = "handbookright";
	rightMenuItem->selectedString = hc;
	rightMenu->items.push_back(rightMenuItem);

	listenerId = manager->inputManager->registerListener(this);
}
Handbook::~Handbook(){
	for(size_t i = 0; i < pages.size(); i++){
		delete pages[i];
		pages.erase(pages.begin() + (i--));
	}
	manager->inputManager->unregisterListener(listenerId);
	manager->menuManager->menus.erase("handbookopen");
	delete hc;
}

void Handbook::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		if(event.button() == sf::Mouse::Left){
			if(*hc == "handbookopen"){
				*hc = "";
				if(isOpen()){
					close();
				}
				if(isClosed()){
					open();
				}
			}
			if(*hc == "handbookclose"){
				*hc = "";
				if(isOpen()){
					close();
				}
			}
			if(*hc == "handbookleft"){
				*hc = "";
				turnLeft();
			}
			if(*hc == "handbookright"){
				*hc = "";
				turnRight();
			}
		}
	}
}

void Handbook::open(){
	if (!isClosed()){
		return;
	}
	currentAnimation = OPENING;
	nextAnimation = OPENED;
	startTime = clock.getElapsedTime();
}
void Handbook::close(){
	if (!isOpen()){
		return;
	}
	currentAnimation = CLOSING;
	nextAnimation = CLOSED;
	startTime = clock.getElapsedTime();
}
bool Handbook::hasLeftPage(){
	return current >= 2;
}
bool Handbook::hasRightPage(){
	return current < pages.size() - 2;
}
void Handbook::turnLeft(){
	if(!hasLeftPage()){
		return;
	}
	current -= 2;
}
void Handbook::turnRight(){
	if(!hasRightPage()){
		return;
	}
	current += 2;
}
bool Handbook::isOpen(){
	return currentAnimation == OPENED;
}
bool Handbook::isClosed(){
	return currentAnimation == CLOSED;
}

void Handbook::render(){
	CoreSprite* cs = getSprite(clock.getElapsedTime());

	if(currentAnimation != CLOSED){
		gi::draw(*cs);
	}

	if(isOpen()){
		if(current < pages.size()){
			gi::draw(
				pages[current]->lines,
				290.0f * gi::dxiz(),
				100.0f * gi::dyiz(),
				640.0f * gi::dxiz(),
				750.0f * gi::dyiz()
				);
		}
		if(current < pages.size() - 1){
			gi::draw(
				pages[current + 1]->lines,
				990.0f * gi::dxiz(),
				100.0f * gi::dyiz(),
				640.0f * gi::dxiz(),
				750.0f * gi::dyiz()
				);
		}
	}

	closeMenu->hidden = !isOpen();
	openMenu->hidden = !isClosed();
	leftMenu->hidden = !isOpen() || !hasLeftPage();
	rightMenu->hidden = !isOpen() || !hasRightPage();
}
