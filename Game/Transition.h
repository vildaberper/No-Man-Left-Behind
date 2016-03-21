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

	bool showOnlyDiary = false;

	sf::Texture* background = nullptr;

	sf::Time renderTime;
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

	void end(const std::string& diary);

	void addPage(Level* level, const float& timeBonus, const float& stress, const size_t& resourceBonus, const std::string& diaryId);

	void render(const float& dt);

	bool hidden = true;
private:
	std::vector<std::string> totalSaved;
	std::vector<std::string> totalLost;

	float ba = 1.0f;
	float bat = 0.0f;
	float bas = 4.0f;
	sf::Texture* background;
	sf::Texture* nextBackground;

	size_t currentRestart = 0;
	std::vector<std::vector<std::string>> restarts;

	sf::Clock start;
	Menu* closeMenu;
	Menu* leftMenu;
	Menu* rightMenu;
	std::string* hc;
	unsigned long listenerId;

	CoreSprite* meter;
	CoreSprite* brain;
	CoreSprite* fill;
	float meterScale = 1.0f;

	CoreSprite* book;
	float bookScale = 1.4f;

	size_t current;
	std::vector<TPage*> pages;

	std::map<std::string, std::vector<std::string>> diaries;

	void stressMeter(const float& stress, const sf::Color& c);

	void paragraphFade(const std::vector<std::string>& text, const sf::Time& time);

	std::string getDiary(const std::string& diary);
};
