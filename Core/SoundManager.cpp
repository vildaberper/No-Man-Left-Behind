#include "SoundManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
}

bool SoundManager::initialize(RenderWindow* window){
	sf::Clock cl;

	undefined = new SoundBuffer();
	File u = c::soundDir.child("undefined.wav");
	if(!u.isFile() || !undefined->loadFromFile(u.path())){
		logger::warning("Undefined sound not found!");
	}
	sounds.insert(undefined);

	// Loading sounds
	if(loadSounds()){
		logger::timing("Sounds initialized in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("Sounds failed to initialize");
		return false;
	}
}

bool SoundManager::finalize(RenderWindow* window){
	// Empty containers
	for(auto &ent : channels){
		delete ent.second;
	}
	channels.clear();
	for(SoundBuffer* sb : sounds){
		delete sb;
	}
	sounds.clear();

	soundBoard.clear();

	return true;
}

void SoundManager::clear(const Entity* source, bool stop){
	if(relative == source){
		relative = nullptr;
	}
	for(auto &ent : channels){
		if(ent.second->source == source){
			ent.second->source = nullptr;
			if(stop){
				ent.second->sound->stop();
			}
		}
	}
}

void SoundManager::tick(RenderWindow* window, const Time& time, const float& dt){
	// Cleaning stopped sounds
	for(auto &ent : channels){
		if(ent.second->sound->getStatus() == Sound::Stopped){
			delete ent.second;
			ent.second = nullptr;
		}
	}
	for(std::map<unsigned long, soundmanager::Sound*>::iterator i = channels.begin(); i != channels.end();){
		if(i->second == nullptr){
			channels.erase(i++);
		}
		else{
			if(relative != nullptr && i->second->source != nullptr){
				Entity* e0 = relative;
				const Entity* e1 = i->second->source;
				float d = math::distance(
					e0->position.x + e0->size.x / 2.0f,
					e0->position.y + e0->size.y / 2.0f,
					e1->position.x + e1->size.x / 2.0f,
					e1->position.y + e1->size.y / 2.0f
					);

				/*
					Directional sound not possible due to stereo sampling.
					Volume is relative.
				*/

				if(d < minDistance){
					i->second->sound->setVolume(100.0f * c::masterVolume * (1.0f - (d / minDistance)));
				}
				else{
					i->second->sound->setVolume(0.0f);
				}
			}
			else{
				i->second->sound->setVolume(100.0f * c::masterVolume);
			}
			if(i->second->hasVolume){
				i->second->sound->setVolume(i->second->sound->getVolume() * i->second->volume);
			}
			++i;
		}
	}

	// If game is paused
	if(dt <= 0.0f){
		if(playing){
			for(auto &ent : channels){
				ent.second->sound->pause();
			}
			playing = false;
		}
	}
	else{
		if(!playing){
			for(auto &ent : channels){
				ent.second->sound->play();
			}
			playing = true;
		}
	}
}

unsigned long SoundManager::play(const Entity* source, const std::string& name, const bool& loop){
	std::string::size_type index = name.find_first_of('.');

	return play(source, name.substr(0, index), name.substr(index + 1), loop);
}
unsigned long SoundManager::playV(const Entity* source, const std::string& name, const float& volume, const bool& loop){
	unsigned long id = play(source, name, loop);

	if(channels.count(id) > 0){
		soundmanager::Sound* s = channels[id];
		s->hasVolume = true;
		s->volume = volume;
	}
	return id;
}
unsigned long SoundManager::playRandom(const Entity* source, const std::string& category, const bool& loop){
	if(soundBoard.count(category) == 0){
		logger::warning("Sound category not found: " + category);
		return 0;
	}
	std::map<std::string, SoundBuffer*> m = soundBoard[category];
	std::map<std::string, SoundBuffer*>::const_iterator i = m.begin();
	std::advance(i, random::randomInt(m.size() - 1));
	return play(
		source,
		category,
		i->first,
		loop
		);
}
unsigned long SoundManager::playRandomV(const Entity* source, const std::string& category, const float& volume, const bool& loop){
	unsigned long id = playRandom(source, category, loop);

	if(channels.count(id) > 0){
		soundmanager::Sound* s = channels[id];
		s->hasVolume = true;
		s->volume = volume;
	}
	return id;
}
unsigned long SoundManager::play(const Entity* source, const string& category, const string& name, const bool& loop){
	unsigned long id = ++idTracker;

	// can add a cap to how many sounds can play at once
	if(channels.size() > 100){
		logger::warning("All them sounds are playing");
		return 0;
	}

	Sound* s = new Sound();
	if(soundBoard.count(category) == 0 || soundBoard[category].count(name) == 0){
		s->setBuffer(*undefined);
		logger::warning("Sound not found: " + category + "." + name);
	}
	else{
		s->setBuffer(*soundBoard[category][name]);
	}
	s->setVolume(c::masterVolume * 100.0f);
	s->setLoop(loop);
	s->setVolume(0.0f);
	s->play();
	channels[id] = new soundmanager::Sound(s, source);
	return id;
}

void SoundManager::setSoundV(const unsigned long& id, const float& volume){
	if(channels.count(id) > 0){
		soundmanager::Sound* s = channels[id];
		s->hasVolume = true;
		s->volume = volume;
	}
}

void SoundManager::stop(const unsigned long& id){
	if(channels.count(id) > 0){
		channels[id]->sound->stop();
	}
}

bool SoundManager::loadSounds(){
	return loadSoundsFromDir(c::soundDir);
}

bool SoundManager::loadSoundsFromDir(File& dir){
	if(!dir.exists() || !dir.isDirectory()){
		return false;
	}
	bool success = true;
	for(File file : dir.listFiles()){
		if(file.isDirectory()){
			success = !loadSoundsFromDir(file) ? false : success;
			continue;
		}

		SoundBuffer* localBuffer = new SoundBuffer();

		if(!localBuffer->loadFromFile(file.path())){
			delete localBuffer;
			logger::warning("Failed to load sound: " + file.path());
			success = false;
			continue;
		}

		// TODO: fix dynamic naming
		String category = file.parent().nameNoExtension();
		String name = file.nameNoExtension();

		// Adding sound to the map and set
		soundBoard[category][name] = localBuffer;
		sounds.insert(localBuffer);
	}
	// All sound loaded
	return success;
}