#include "MusicManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

bool DEFAULT_FADEOUT = false;
bool DEFAULT_FADEIN = false;
bool DEFAULT_LOOP = false;

MusicManager::MusicManager(){
}

MusicManager::~MusicManager(){
}

bool MusicManager::initialize(sf::RenderWindow* window){
	Clock cl;
	// Loading sounds
	clock.restart();
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
		if (id.second.music->getStatus() == sf::Music::Stopped){
			channels.erase(id.first);
		}
	}
	// check channels mm::Music for special treatments
	for (auto &id : channels){
		Time elapsed = time - channels[id.first].start;

		if (id.second.fadeIn && elapsed <= FADE_DURATION){	// Fading in
			float v = elapsed / FADE_DURATION;
			id.second.music->setVolume(v * c::musicVolume * c::masterVolume * 100.0f);
		}
		else if (id.second.fadeOut && id.second.duration.asMilliseconds() != 0 && elapsed >= id.second.duration - FADE_DURATION){	// Fading out
			if (elapsed >= id.second.duration){
				id.second.music->stop();
				continue;
			}
			
			float v = 1 - ((id.second.duration - elapsed) / FADE_DURATION);

			id.second.music->setVolume(v * c::musicVolume * c::masterVolume * 100.0f);
		}
		else{
			id.second.music->setVolume(c::musicVolume * c::masterVolume * 100.0f);
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
	unsigned long id = idTracker++;
	if (musicBoard.count(category) == 0 || musicBoard[category].count(name) == 0){
		// TODO: call undefined sound
		logger::warning("Music " + category + "." + name + "could not be found.");
		return 0;
	}
	else{
		mm::Music localMusic;

		localMusic.music = new Music();
		if (!localMusic.music->openFromFile(musicBoard[category][name]->path())){
			logger::warning("File " + musicBoard[category][name]->path() + "could not be found.");
		}
		localMusic.music->setVolume(0.0f);

		localMusic.fadeIn = fadeIn;
		localMusic.fadeOut = fadeOut;
		localMusic.loop = loop;
		localMusic.music->setLoop(loop);

		if (loop){
			localMusic.duration = milliseconds(0);
		}
		else{
			localMusic.duration = localMusic.music->getDuration();
		}

		localMusic.music->play();
		localMusic.start = clock.getElapsedTime();
		channels[id] = localMusic;

		return id;
	}
}

void MusicManager::stop(const unsigned long& id, const bool& force){
	if (channels.count(id) == 0){
		return;
	}

	mm::Music m = channels[id];

	if (force){
		m.music->stop();
	}
	else{
		m.duration = clock.getElapsedTime() - m.start + FADE_DURATION;
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
			continue;
		}

		if (!Music().openFromFile(file.path())){
			logger::warning("Skipping music: " + file.parent().name() + "\\" + file.name());
			continue;
		}

		// Adding music to the archive
		File* localFile = new File(file.path());

		musicBoard[file.parent().name()][file.nameNoExtension()] = localFile;
		musicArchived.insert(localFile);
	}
	// All music archived
	return success;
}