#pragma once

#include <map>
#include <iterator>

#include "File.h"
#include "SFMLI.h"
#include "Entity.h"
#include "Random.h"
#include "MathHelper.h"

namespace soundmanager{
	class Sound{
	public:
		Sound(sf::Sound* sound, const Entity* source){
			Sound::sound = sound;
			Sound::source = source;
		}
		~Sound(){
			if(sound->getStatus() != sf::Sound::Stopped){
				sound->stop();
			}
			delete sound;
		}
		sf::Sound* sound;
		const Entity* source = NULL;
	};
}

class SoundManager{
public:
	SoundManager();
	~SoundManager();

	bool initialize(sf::RenderWindow* window);
	bool finalize(sf::RenderWindow* window);

	void clear(const Entity* source, bool stop = true);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	unsigned long play(const Entity* source, const std::string& name, const bool& loop);
	unsigned long playRandom(const Entity* source, const std::string& category, const bool& loop);
	unsigned long play(const Entity* source, const std::string& category, const std::string& name, const bool& loop);
	void stop(const unsigned long& id);

	Entity* relative = NULL;
	float minDistance = 1500.0f;
private:
	std::map<std::string, std::map<std::string, sf::SoundBuffer*>> soundBoard;
	std::set<sf::SoundBuffer*> sounds;
	std::map<unsigned long, soundmanager::Sound*> channels;

	sf::SoundBuffer* undefined;

	bool loadSounds();
	bool loadSoundsFromDir(File& dir);

	bool playing = false;
	unsigned long idTracker = 0;
};