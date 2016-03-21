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

	meter = manager->spriteManager->getSprite("stress.meter");
	brain = manager->spriteManager->getSprite("stress.brain");
	fill = manager->spriteManager->getSprite("stress.fill");

	book = manager->spriteManager->getSprite("handbook.state1", "0");
	reset();
	hc = new std::string();

	closeMenu = new Menu();
	closeMenu->position = Vector(1580.0f, 100.0f);
	closeMenu->size = Vector(50.0f, 50.0f);
	closeMenu->hidden = true;
	closeMenu->type = HORIZONTAL;
	closeMenu->background = new TexBar(
		nullptr,
		manager->spriteManager->getTexture("book.close"),
		nullptr
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
		nullptr,
		manager->spriteManager->getTexture("book.turnleft"),
		nullptr
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
		nullptr,
		manager->spriteManager->getTexture("book.turnright"),
		nullptr
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
		nextBackground = pages[current]->background;
	}
}
void Transition::turnRight(){
	if(current < pages.size() - 1){
		current++;
		nextBackground = pages[current]->background;
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
	p->showOnlyDiary = true;
	pages.push_back(p);

	background = nextBackground = nullptr;
	bat = ba = 0.0f;

	current = 0;
	totalSaved.clear();
}

void Transition::end(const std::string& diary){
	TPage* p = new TPage();
	p->lines.push_back("outro");
	p->diary = getDiary(diary);
	p->showStressMeter = false;
	p->showOnlyDiary = true;
	pages.push_back(p);

	current = pages.size() - 1;
	nextBackground = p->background;
}

void Transition::addPage(Level* level, const float& timeBonus, const float& stress, const size_t& resourceBonus, const std::string& diaryId){
	TPage* p = new TPage();
	p->stress = stress;

	p->background = level->world->background;

	std::vector<std::string> saved;
	std::vector<std::string> lost;

	for(size_t i = 0; i < level->injured.size(); i++){
		Injured* inj = level->injured[i];
		if(inj->survived()){
			saved.push_back(inj->name);
			totalSaved.push_back(inj->name);
		}
		else{
			lost.push_back(inj->name);
			totalLost.push_back(inj->name);
		}
	}

	std::string diary = diaryId;

	if(diaryId.at(0) == '-'){
		diary = diary.substr(1);

		std::vector<std::string> end;
		std::string line = "   ";
		bool first = true;

		end.push_back("");
		end.push_back("Summary");

		if(totalSaved.size() > 0){
			end.push_back("");
			end.push_back("Saved:");
			for(std::string name : totalSaved){
				if(line.length() + 2 + name.length() <= 70){
					line += (first ? "" : ", ") + name;
				}
				else{
					end.push_back(line);
					line = "   " + name;
				}
				first = false;
			}
			end.push_back(line + ".");
		}


		if(totalLost.size() > 0){
			end.push_back("");
			end.push_back("Lost:");
			line = "   ";
			first = true;
			for(std::string name : totalLost){
				if(line.length() + 2 + name.length() <= 70){
					line += (first ? "" : ", ") + name;
				}
				else{
					end.push_back(line);
					line = "   " + name;
				}
				first = false;
			}
			end.push_back(line + ".");
		}
		diaries["end"] = end;
		p->diary = "end";
	}
	else{
		p->diary = getDiary(diary);
	}

	if(diary.length() > 0){
		p->lines.push_back("");
		p->lines.push_back("Year " + diary.substr(0, 1));
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

	pages.push_back(p);
	current = pages.size() - 1;
	background = nullptr;
	nextBackground = p->background;
}

void Transition::render(const float& dt){
	leftMenu->hidden = hidden || current == 0;
	rightMenu->hidden = hidden || current >= pages.size() - 1;
	closeMenu->hidden = hidden || pages[current]->showOnlyDiary;

	TPage* p = pages[current];

	p->renderTime += sf::seconds(dt);

	if(ba > bat && ba > 0.0f){
		ba -= dt * bas;
	}
	else if(ba < bat && ba < 1.0f){
		ba += dt * bas;
	}
	if(ba < 0.0f){
		ba = 0.0f;
	}
	else if(ba > 1.0f){
		ba = 1.0f;
	}

	if(background != nextBackground){
		bat = 1.0f;
		if(background == nullptr){
			ba = 1.0f;
		}
		if(ba >= 1.0f && bat >= 1.0f){
			background = nextBackground;
		}
	}
	else{
		bat = 0.0f;
	}

	if(background != nullptr){
		sf::Sprite s = sf::Sprite(*background, sf::IntRect(0, 0, int(gi::WIDTH + 3 * background->getSize().x), int(gi::HEIGHT + 3 * background->getSize().y)));
		float x = (-gi::cameraX + gi::WIDTH / 2) * gi::dx();
		float y = (-gi::cameraY + gi::HEIGHT / 2) * gi::dy();
		x = fmod(x, background->getSize().x * gi::dx());
		y = fmod(y, background->getSize().y * gi::dy());
		s.setPosition(x - background->getSize().x * gi::dx(), y - background->getSize().y * gi::dy());
		s.scale(gi::dx(), gi::dy());
		gi::draw(s);

		gi::darken(ba);
	}

	if(pages[current]->showOnlyDiary){
		paragraphFade(
			diaries[p->diary], p->renderTime);
	}
	else{
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
			p->lines,
			290.0f * gi::dxiz(),
			100.0f * gi::dyiz(),
			640.0f * gi::dxiz(),
			750.0f * gi::dyiz()
			);

		gi::draw(
			diaries[p->diary],
			990.0f * gi::dxiz(),
			100.0f * gi::dyiz(),
			640.0f * gi::dxiz(),
			750.0f * gi::dyiz()
			);

		if(p->showStressMeter){
			float sm = (p->renderTime.asSeconds() / 2.0f);
			sm = sm > 1.0f ? 1.0f : sm;
			sm = 1.0f - sm + p->stress;
			sm = sm > 1.0f ? 1.0f : (sm < 0.0f ? 0.0f : sm);

			stressMeter(sm, c);
		}
	}
}

void Transition::stressMeter(const float& stress, const sf::Color& c){
	float x = 750 * gi::dxiz();
	float y = 750 * gi::dyiz();
	float sx = meterScale * gi::dxiz();
	float sy = meterScale * gi::dyiz();

	float fillStart = 46.0f / 500.0f;
	float fillEnd = 322.0f / 500.0f;
	float fillProgress = (fillStart + (fillEnd - fillStart) * stress);

	brain->sprite()->setPosition(x, y);
	sf::Uint8 bru = sf::Uint8(int(255.0f * stress));
	sf::Color brc(bru, bru, bru, 255);
	brain->sprite()->setColor(brc);
	brain->sprite()->scale(
		(1.0f / brain->sprite()->getScale().x) * sx,
		(1.0f / brain->sprite()->getScale().y) * sy
		);
	gi::renderWindow->draw(*brain->sprite());

	fill->sprite()->setPosition(x, y + (fill->h() * fillProgress) * sy);
	fill->sprite()->setTextureRect(sf::IntRect(0, int(fill->h() * fillProgress), fill->w(), int(fill->h() * (1.0f - fillProgress))));
	fill->sprite()->scale(
		(1.0f / fill->sprite()->getScale().x) * sx,
		(1.0f / fill->sprite()->getScale().y) * sy
		);
	gi::renderWindow->draw(*fill->sprite());

	meter->sprite()->setPosition(x, y);
	meter->sprite()->scale(
		(1.0f / meter->sprite()->getScale().x) * sx,
		(1.0f / meter->sprite()->getScale().y) * sy
		);
	gi::renderWindow->draw(*meter->sprite());
}

void Transition::paragraphFade(const std::vector<std::string>& text, const sf::Time& time){
	float x = (gi::TARGET_WIDTH / 2.0f - 320.0f) * gi::dxiz();
	float y = 200.0f * gi::dyiz();
	float w = 640.0f * gi::dxiz();
	float h = 750.0f * gi::dyiz();

	float dh = std::min(20.0f, h / float(text.size()));
	float dhh = dh + 10.0f;

	size_t ps = 0;
	size_t mps = size_t(floor(time.asSeconds() / gc::textFadeDuration));

	float ca = 1.0f;
	float cf = fmod(time.asSeconds() / gc::textFadeDuration, 1.0f);

	for(size_t i = 0; i < text.size() && ps <= mps; i++){
		std::string s = text[i];

		size_t index;
		if((index = s.find("%saved%")) != std::string::npos){
			s.replace(index, 7, std::to_string(totalSaved.size()));
		}

		if(text[i].length() == 0){
			ps++;
			if(ps == mps){
				ca = cf;
			}
			else{
				ca = 1.0f;
			}
		}

		sf::Text title = sf::Text();
		title.setFont(gi::textFont);
		title.setColor(sf::Color(255, 255, 255, sf::Uint8(int(255.0f * ca))));
		title.setString(s);
		title.setCharacterSize(30);
		float sc = std::min(0.6f, w / (title.getGlobalBounds().width + 10));
		title.scale(sc, sc);
		title.setOrigin(0, 0);
		title.setPosition(x + 5 * gi::dxiz(), y + i * dhh * gi::dyiz() - 5 * gi::dyiz());
		gi::renderWindow->draw(title);
	}
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