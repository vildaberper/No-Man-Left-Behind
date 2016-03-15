#pragma once

#include <vector>

#include "GI.h"
#include "Manager.h"
#include "Animatable.h"
#include "GameConstants.h"

class Page{
public:
	Page(){

	}
	~Page(){

	}

	std::vector<std::string> lines;
};

namespace handbook{
	static std::string CLOSED = "state0";
	static std::string OPENED = "state1";
	static std::string CLOSING = "state2";
	static std::string OPENING = "state3";
}

class Handbook : public Animatable, public InputListener{
public:
	Handbook(Manager* m);
	~Handbook();

	void on(MouseButtonEvent& event);

	void open();
	void close();
	void forceClose();
	bool hasLeftPage();
	bool hasRightPage();
	void turnLeft();
	void turnRight();
	bool isOpen();
	bool isClosed();

	void render();

	std::vector<Page*> pages;
	size_t current;

	Manager* manager;

	sf::Clock clock;

	Menu* openMenu;
	Menu* closeMenu;
	Menu* leftMenu;
	Menu* rightMenu;
	std::string* hc;

	unsigned long listenerId;
};
