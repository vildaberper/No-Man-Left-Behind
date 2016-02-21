#pragma once

#include "SFMLI.h"
#include "File.h"

class SoundManager{
public:
	SoundManager();
	~SoundManager();

	bool initialize(sf::RenderWindow* window);
	bool finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	unsigned long play(const std::string& name, const bool& loop);
	unsigned long play(const std::string& category, const std::string& name, const bool& loop);
	void stop(const unsigned long& id);
private:
	std::map<std::string, std::map<std::string, sf::SoundBuffer*>> soundBoard;
	std::set<sf::SoundBuffer*> sounds;
	std::map<unsigned long, sf::Sound*> channels;

	sf::SoundBuffer* undefined;

	bool loadSounds();
	bool loadSoundsFromDir(File& dir);

	bool playing = false;
	unsigned long idTracker = 0;
};