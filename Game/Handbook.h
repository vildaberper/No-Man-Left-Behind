#pragma once

#include <vector>

#include "GI.h"
#include "Manager.h"
#include "Animatable.h"

class Page{
public:
	Page();
	~Page();

	std::vector<std::string> lines;
};

namespace handbook{
	static std::string CLOSED = "state0";
	static std::string OPENED = "state1";
	static std::string CLOSING = "state2";
	static std::string OPENING = "state3";
}

class Handbook : public Animatable{
public:
	Handbook(Manager* m);
	~Handbook();

	void openHandbook(const sf::Time& time);
	void closeHandbook(const sf::Time& time);
	bool hasLeftPage();
	bool hasRightPage();
	void turnLeft();
	void turnRight();
	bool isOpen();
	bool isClosed();

	void render(const sf::Time& time);

	std::vector<Page*> pages;
	size_t current;
};
