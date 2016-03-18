#pragma once

#include <map>
#include <string>
#include <vector>

#include "Level.h"
#include "Manager.h"
#include "FileHelper.h"
#include "GameConstants.h"

class TPage{
public:
	TPage(){

	}
	~TPage(){

	}

	std::vector<std::string> lines;
	float stress;
	std::string diary;
	bool showStressMeter = true;
};

class Transition : public InputListener{
public:
	Transition(Manager* manager);
	~Transition();

	void newRestart();

	void renderRestart();

	void on(MouseButtonEvent& event);

	void setHidden(const bool& hidden);

	void turnLeft();
	void turnRight();

	void reset();

	void end(const float& stress);

	void addPage(Level* level, const float& timeBonus, const float& stress, const size_t& resourceBonus, const std::string& diary);

	void render();

	bool hidden = true;
private:
	size_t currentRestart = 0;
	std::vector<std::vector<std::string>> restarts;

	sf::Clock start;
	Menu* closeMenu;
	Menu* leftMenu;
	Menu* rightMenu;
	std::string* hc;
	unsigned long listenerId;

	CoreSprite* skull;
	float skullScale = 0.25f;

	CoreSprite* book;
	float bookScale = 1.4f;

	size_t current;
	std::vector<TPage*> pages;

	std::map<std::string, std::vector<std::string>> diaries;

	void stressMeter(const float& stress, const sf::Color& c);

	std::string getDiary(const std::string& diary);
};
