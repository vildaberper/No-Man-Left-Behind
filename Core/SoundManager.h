#pragma once

#include "SFMLI.h"

class SoundManager{
public:
	SoundManager();
	~SoundManager();

	bool initialize();

	bool finalize();

	bool tick(const sf::Time& time, const float& dt);

	bool play(const string& category, const string& name);

private:
	std::map<std::string, std::map<std::string, sf::SoundBuffer*>> soundBoard;
	std::set<sf::SoundBuffer*> sounds;
	std::vector<sf::Sound*> channels;

	bool loadSound();
	bool loadSoundFromDir(File& dir);
};