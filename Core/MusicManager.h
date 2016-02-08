#pragma once

#include "SFMLI.h"
#include "File.h"

static sf::Time FADE_DURATION = sf::milliseconds(1000);

namespace mm{
	class Music{
		File file;
		sf::Music* music;

		bool fadeIn;
		bool fadeOut;
		bool loop;

		sf::Time start;
		sf::Time duration;
	};
}

class MusicManager{
public:
	MusicManager();
	~MusicManager();

	bool initialize(sf::RenderWindow* window);
	bool finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	unsigned long play(const std::string& name);
	unsigned long play(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);
	unsigned long play(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);

	void stop(const unsigned long& id, const bool& force);
private:
	std::map<std::string, std::map<std::string, File*>> musicBoard;
	std::map<unsigned long, mm::Music> channels;
};