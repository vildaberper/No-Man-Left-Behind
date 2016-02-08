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

	bool play(const std::string& category, const std::string& name);
private:
	std::map<std::string, std::map<std::string, sf::SoundBuffer*>> soundBoard;
	std::set<sf::SoundBuffer*> sounds;
	std::vector<sf::Sound*> channels;

	bool playing;	// Flag to signify if sounds are being played or paused

	bool loadSounds();
	bool loadSoundsFromDir(File& dir);
};