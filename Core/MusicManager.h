#pragma once

#include "SFMLI.h"
#include "File.h"

static sf::Time FADE_DURATION = sf::milliseconds(2000);

namespace mm{
	class Music{
	public:
		Music(){

		}
		~Music(){
			if(music->getStatus() != sf::Music::Stopped){
				music->stop();
			}
			delete music;
		}
		File file;
		sf::Music* music;

		bool fadeOut;
		bool fadeIn;
		bool loop;

		sf::Time duration;
		sf::Time start;
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
	std::map<unsigned long, mm::Music*> channels;
	std::set<File*> musicArchived;

	bool archiveMusic();
	bool archiveMusicFromDir(File& dir);

	unsigned long idTracker = 0;

	sf::Clock clock;
};