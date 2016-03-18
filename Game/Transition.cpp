#include "Transition.h"

Transition::Transition(Manager* manager){
	for(File f : gc::transitionDir.listFiles()){
		if(!f.isFile() || f.extension() != "txt"){
			continue;
		}

		std::vector<std::string>* lines = f.readTextFile();
		diaries[f.nameNoExtension()] = *lines;
	}
	std::vector<std::string> def;
	def.push_back("I don't remember what I did today...");
	diaries["default"] = def;

	std::vector<std::string>* lines = gc::restartFile.readTextFile();
	std::vector<std::string> restart;
	for(size_t i = 0; i < lines->size(); i++){
		std::string line = (*lines)[i];

		if(line == "%%"){
			restarts.push_back(restart);
			restart = std::vector<std::string>();
			continue;
		}

		restart.push_back(line);
	}
	restarts.push_back(restart);
	delete lines;

	skull = manager->spriteManager->getSprite("timer.skull");
	book = manager->spriteManager->getSprite("handbook.state1", "0");
	reset();
	hc = new std::string();

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
	manager->menuManager->menus["transitionclose"] = closeMenu;
	MenuItem* closeMenuItem = new MenuItem();
	closeMenuItem->selectedPrefix = "transitionclose";
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
	manager->menuManager->menus["transitionleft"] = leftMenu;
	MenuItem* leftMenuItem = new MenuItem();
	leftMenuItem->selectedPrefix = "transitionleft";
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
	manager->menuManager->menus["transitionright"] = rightMenu;
	MenuItem* rightMenuItem = new MenuItem();
	rightMenuItem->selectedPrefix = "transitionright";
	rightMenuItem->selectedString = hc;
	rightMenu->items.push_back(rightMenuItem);

	listenerId = manager->inputManager->registerListener(this);
}

Transition::~Transition(){
	delete hc;
}

void Transition::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		if(event.button() == sf::Mouse::Left){
			if(*hc == "transitionclose"){
				*hc = "";
				setHidden(true);
			}
			if(*hc == "transitionleft"){
				*hc = "";
				turnLeft();
			}
			if(*hc == "transitionright"){
				*hc = "";
				turnRight();
			}
		}
	}
}

void Transition::newRestart(){
	currentRestart = random::randomInt(restarts.size() - 1);
}

void Transition::renderRestart(){
	gi::darken(1.0f);
	sf::Text t;
	t.setFont(gi::textFont);
	t.setColor(sf::Color(255, 255, 255, 255));
	t.setCharacterSize(unsigned int(gi::dy() * 40));

	float step = t.getCharacterSize() + 10.0f;
	float start = gi::wy() / 2 - step * float(restarts[currentRestart].size()) / 2.0f;
	for(size_t i = 0; i < restarts[currentRestart].size(); i++){
		t.setString(restarts[currentRestart][i]);
		t.setPosition(gi::wx() / 2 - t.getGlobalBounds().width / 2, start + step * i);
		gi::renderWindow->draw(t);
	}
}

void Transition::setHidden(const bool& hidden){
	Transition::hidden = hidden;

	if(hidden){
		closeMenu->hidden = true;
		leftMenu->hidden = true;
		rightMenu->hidden = true;
	}
	else{
		start.restart();
	}
}

void Transition::turnLeft(){
	if(current > 0){
		current--;
	}
}
void Transition::turnRight(){
	if(current < pages.size() - 1){
		current++;
	}
}

void Transition::reset(){
	for(TPage* p : pages){
		delete p;
	}
	pages.clear();
	
	TPage* p = new TPage();
	p->lines.push_back("intro");
	p->diary = getDiary("0a");
	p->showStressMeter = false;
	pages.push_back(p);

	current = 0;
}

void Transition::end(const float& stress){
	TPage* p = new TPage();
	p->lines.push_back("outro");
	pages.push_back(p);
	p->showStressMeter = false;

	current = pages.size() - 1;
}

void Transition::addPage(Level* level, const float& timeBonus, const float& stress, const size_t& resourceBonus, const std::string& diary){
	TPage* p = new TPage();
	p->stress = stress;

	std::vector<std::string> saved;
	std::vector<std::string> lost;

	for(size_t i = 0; i < level->injured.size(); i++){
		Injured* inj = level->injured[i];
		if(inj->survived()){
			saved.push_back(inj->name);
		}
		else{
			lost.push_back(inj->name);
		}
	}

	std::string line = "   ";
	bool first = true;

	if(saved.size() > 0){
		p->lines.push_back("");
		p->lines.push_back("Saved:");
		for(std::string name : saved){
			if(line.length() + 2 + name.length() <= 70){
				line += (first ? "" : ", ") + name;
			}
			else{
				p->lines.push_back(line);
				line = "   " + name;
			}
			first = false;
		}
		p->lines.push_back(line + ".");
	}


	if(lost.size() > 0){
		p->lines.push_back("");
		p->lines.push_back("Lost:");
		line = "   ";
		first = true;
		for(std::string name : lost){
			if(line.length() + 2 + name.length() <= 70){
				line += (first ? "" : ", ") + name;
			}
			else{
				p->lines.push_back(line);
				line = "   " + name;
			}
			first = false;
		}
		p->lines.push_back(line + ".");
	}

	if(timeBonus > 0.0f){
		p->lines.push_back("");
		p->lines.push_back("Bonus time: " + std::to_string(int(round(timeBonus))));
	}

	if(resourceBonus > 0){
		p->lines.push_back("");
		p->lines.push_back("Bonus resources: " + std::to_string(resourceBonus));
	}

	p->diary = getDiary(diary);

	pages.push_back(p);
	current = pages.size() - 1;
}

void Transition::render(){
	/*float t = start.getElapsedTime().asSeconds();

	if(t > 1.0f){
		leftMenu->hidden = current == 0;
		rightMenu->hidden = current >= pages.size() - 1;
		closeMenu->hidden = false;
	}*/
	leftMenu->hidden = hidden || current == 0;
	rightMenu->hidden = hidden || current >= pages.size() - 1;
	closeMenu->hidden = hidden || false;

	sf::Color c = sf::Color(255, 255, 255, 255);
	book->sprite()->setPosition(
		(gi::TARGET_WIDTH / 2.0f - (book->w() * bookScale / 2.0f)) * gi::dxiz(),
		(gi::TARGET_HEIGHT / 2.0f - (book->h() * bookScale / 2.0f)) * gi::dyiz()
		);
	book->sprite()->scale(
		(1.0f / book->sprite()->getScale().x) * bookScale * gi::dxiz(),
		(1.0f / book->sprite()->getScale().y) * bookScale * gi::dyiz()
		);
	book->sprite()->setColor(c);
	gi::renderWindow->draw(*book->sprite());

	gi::draw(
		pages[current]->lines,
		290.0f * gi::dxiz(),
		100.0f * gi::dyiz(),
		640.0f * gi::dxiz(),
		750.0f * gi::dyiz()
		);

	gi::draw(
				diaries[pages[current]->diary],
				990.0f * gi::dxiz(),
				100.0f * gi::dyiz(),
				640.0f * gi::dxiz(),
				750.0f * gi::dyiz()
				);

	if(pages[current]->showStressMeter){
		stressMeter(pages[current]->stress, c);
	}
}

void Transition::stressMeter(const float& stress, const sf::Color& c){
	float v = 1.0f - stress;
	if(v < 0.0f){
		v = 0.0f;
	}
	else if(v > 1.0f){
		v = 1.0f;
	}
	float w = 150 * gi::dxiz();
	float h = 75 * gi::dyiz();
	sf::ConvexShape t;
	t.setPosition(750 * gi::dxiz(), 750 * gi::dyiz());
	t.setFillColor(sf::Color(255, 255, 255, c.a));
	t.setOutlineColor(sf::Color(0, 0, 0, c.a));
	t.setOutlineThickness(7 * gi::dxiz());
	t.setPointCount(3);
	t.setPoint(0, sf::Vector2f(0, 0));
	t.setPoint(1, sf::Vector2f(w, 0));
	t.setPoint(2, sf::Vector2f(0, h));
	gi::renderWindow->draw(t);

	t.setFillColor(sf::Color(104, 24, 24, c.a));
	t.setPoint(0, sf::Vector2f(0, h * (1.0f - v)));
	t.setPoint(1, sf::Vector2f(w * v, h * (1.0f - v)));
	t.setPoint(2, sf::Vector2f(0, h));
	gi::renderWindow->draw(t);

	skull->sprite()->setPosition(t.getPosition());
	skull->sprite()->move(-25.0f * gi::dxiz(), 4.0f * gi::dyiz());
	skull->sprite()->scale(
		(1.0f / skull->sprite()->getScale().x) * skullScale * gi::dxiz(),
		(1.0f / skull->sprite()->getScale().y) * skullScale * gi::dyiz()
		);
	skull->sprite()->setColor(c);
	gi::renderWindow->draw(*skull->sprite());
}

std::string Transition::getDiary(const std::string& diary){
	if(diary.size() > 1){
		if(diaries.count(diary) > 0){
			return diary;
		}
		switch(diary.at(1)){
		case 'c':
			return getDiary(diary.substr(0, 1) + "b");
			break;
		case 'b':
			return getDiary(diary.substr(0, 1) + "a");
			break;
		}
	}
	return "default";
}