#pragma once

#include "SFMLI.h"
#include "File.h"

class MusicManager{
public:
	MusicManager();
	~MusicManager();

	bool initialize(sf::RenderWindow* window);
	bool finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	bool play(const std::string& category, const std::string& name);

	bool fadeTo(const std::string& category, const std::string& name);

	bool fadeOut();

private:
	sf::Music* channelA;
	sf::Music* channelB;
};

