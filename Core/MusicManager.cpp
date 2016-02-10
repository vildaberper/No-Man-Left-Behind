#include "MusicManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

bool DEFAULT_FADEOUT = false;
bool DEFAULT_FADEIN = true;
bool DEFAULT_LOOP = true;

MusicManager::MusicManager(){
}

MusicManager::~MusicManager(){
}

bool MusicManager::initialize(sf::RenderWindow* window){
	Clock cl;
	// Loading sounds
	if (archiveMusic()){
		logger::timing("Music loaded in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("MusicManager failed to initialize");
		return false;
	}
}

bool MusicManager::finalize(sf::RenderWindow* window){
	// Empty containers
	for (File* f : musicArchived){
		delete f;
	}
	musicArchived.clear();

	musicBoard.clear();

	channels.clear();

	return true;
}

void MusicManager::tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){
	// GarbageDAY!
	for (auto &id : channels){
		if (channels[id.first].music->getStatus() == sf::Music::Stopped){
			channels.erase(id.first);	// Not sure if this is legit
		}
	}
	// check channels mm::Music for special treatments
	for (auto &id : channels){
		// Fading out
		if (channels[id.first].fadeIn){
			if (channels[id.first].start == sf::Time::Zero){
				channels[id.first].start = time;
			}

			Time elapsed = time - channels[id.first].start;
			
			float v = elapsed / FADE_DURATION;
			if (v > 100.0f){
				v = 100.0f;

				channels[id.first].fadeIn = false;
			}

			channels[id.first].music->setVolume(c::masterVolume * v);
		}
		// Fading Out
		if (channels[id.first].fadeOut){
			if (channels[id.first].start == sf::Time::Zero){
				channels[id.first].start = time;
			}

			Time elapsed = time - channels[id.first].start;
			
			float v = 1 - (elapsed / FADE_DURATION);
			if (v < 0.0f){
				v = 0.0f;

				channels[id.first].music->stop();
				channels[id.first].fadeOut = false;
			}

			channels[id.first].music->setVolume(c::masterVolume * v);
		}
	}
}

unsigned long MusicManager::play(const std::string& name){
	std::string::size_type index = name.find_first_of('.');

	return play(name.substr(0, index), name.substr(index + 1), DEFAULT_FADEIN, DEFAULT_FADEOUT, DEFAULT_LOOP);
}

unsigned long MusicManager::play(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	std::string::size_type index = name.find_first_of('.');

	return play(name.substr(0, index), name.substr(index + 1), fadeIn, fadeOut, loop);
}

unsigned long MusicManager::play(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	if (!channels.size() > 100){
		if (musicBoard.count(category) == 0 || musicBoard[category].count(name) == 0){
			// TODO: call undefined sound
			logger::warning("Music " + category + "." + name + "could not be found.");
			return 0;
		}
		else{
			unsigned long id = 0;
			while (channels.count(id) == 0){
				id++;
			}

			mm::Music localMusic;

			localMusic.music = new Music();
			localMusic.music->openFromFile(musicBoard[category][name]->name());

			localMusic.fadeIn = fadeIn;
			localMusic.fadeOut = fadeOut;
			localMusic.loop = loop;

			localMusic.duration = FADE_DURATION;

			channels[id] = localMusic;
			return id;
		}
	}
	else{
		// Channels caped
		logger::warning("All them songs are playing");
		return false;
	}
}

void MusicManager::stop(const unsigned long& id, const bool& force){
	if (force){
		channels[id].music->stop();
	}
	else{
		channels[id].fadeOut = true;
	}
}

bool MusicManager::archiveMusic(){
	return archiveMusicFromDir(c::musicDir);
}

bool MusicManager::archiveMusicFromDir(File& dir){
	if (!dir.exists() || !dir.isDirectory()){
		return false;
	}

	bool success = true;

	for (File file : dir.listFiles()){
		if (file.isDirectory()){
			success = !archiveMusicFromDir(file) ? false : success;
		}
		// Adding music to the archive
		File* localFile = new File(); *localFile = file;

		musicBoard[file.parent().name()][file.nameNoExtension()] = localFile;
		musicArchived.insert(localFile);

		logger::info("Music " + file.parent().name() + "." + file.name() + " loaded.");
	}
	// All music archived
	return success;
}