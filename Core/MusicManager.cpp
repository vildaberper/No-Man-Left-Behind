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
	if(archiveMusic()){
		logger::timing("Music initialized in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("Music failed to initialize");
		return false;
	}
}

bool MusicManager::finalize(sf::RenderWindow* window){

	// Empty containers
	for(File* f : musicArchived){
		delete f;
	}
	for(auto &id : channels){
		delete id.second;
	}
	channels.clear();
	musicArchived.clear();
	musicBoard.clear();

	return true;
}

void MusicManager::tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){
	// GarbageDAY!
	std::map<unsigned long, mm::Music*>::iterator itr = channels.begin();
	while(itr != channels.end()){
		std::pair<unsigned long, mm::Music*> mp = (*itr);
		if(!mp.second->queued && mp.second->music->getStatus() == sf::Music::Stopped){
			if(mp.second->next != 0 && channels.count(mp.second->next) != 0){
				mm::Music* m = channels[mp.second->next];

				m->queued = false;
				m->start = clock.getElapsedTime();
				m->music->play();
			}
			delete (*itr).second;
			itr = channels.erase(itr);
		}
		else{
			++itr;
		}
	}

	// check channels mm::Music for special treatments
	for(auto &id : channels){
		Time elapsed = clock.getElapsedTime() - id.second->start;

		if(elapsed > id.second->duration && id.second->duration.asMilliseconds() != 0){
			id.second->music->stop();
		}
		else if(id.second->fadeIn && elapsed <= FADE_DURATION){	// Fading in
			float v = elapsed / FADE_DURATION;
			id.second->music->setVolume(v * c::musicVolume * c::masterVolume * 100.0f);
		}
		else if(id.second->fadeOut && id.second->duration.asMilliseconds() != 0 && elapsed >= id.second->duration - FADE_DURATION){	// Fading out
			float v = (id.second->duration - elapsed) / FADE_DURATION;

			id.second->music->setVolume(v * c::musicVolume * c::masterVolume * 100.0f);
		}
		else{
			id.second->music->setVolume(c::musicVolume * c::masterVolume * 100.0f);
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
	unsigned long id = ++idTracker;
	if(musicBoard.count(category) == 0 || musicBoard[category].count(name) == 0){
		logger::warning("Music " + category + "." + name + " could not be found.");
		return 0;
	}
	else{
		mm::Music* localMusic = new mm::Music();

		localMusic->music = new Music();
		if(!localMusic->music->openFromFile(musicBoard[category][name]->path())){
			logger::warning("File " + musicBoard[category][name]->path() + " could not be found.");
		}
		localMusic->music->setVolume(0.0f);

		localMusic->fadeIn = fadeIn;
		localMusic->fadeOut = fadeOut;
		localMusic->loop = loop;
		localMusic->music->setLoop(loop);

		if(loop){
			localMusic->duration = milliseconds(0);
		}
		else{
			localMusic->duration = localMusic->music->getDuration();
		}

		localMusic->music->play();
		localMusic->start = clock.getElapsedTime();
		channels[id] = localMusic;

		return id;
	}
}
unsigned long MusicManager::queue(const unsigned long& before, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	std::string::size_type index = name.find_first_of('.');

	return queue(before, name.substr(0, index), name.substr(index + 1), fadeIn, fadeOut, loop);
}
// Stupid redundance, I know...
unsigned long MusicManager::queue(const unsigned long& before, const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	unsigned long id = ++idTracker;
	if(musicBoard.count(category) == 0 || musicBoard[category].count(name) == 0){
		logger::warning("Music " + category + "." + name + " could not be found.");
		return 0;
	}
	else if(channels.count(before) == 0){
		logger::warning("No sound is currently playing with the id: " + std::to_string(before));
		return 0;
	}
	else{
		mm::Music* localMusic = new mm::Music();

		localMusic->music = new Music();
		if(!localMusic->music->openFromFile(musicBoard[category][name]->path())){
			logger::warning("File " + musicBoard[category][name]->path() + " could not be found.");
		}
		localMusic->music->setVolume(0.0f);

		localMusic->fadeIn = fadeIn;
		localMusic->fadeOut = fadeOut;
		localMusic->loop = loop;
		localMusic->music->setLoop(loop);

		if(loop){
			localMusic->duration = milliseconds(0);
		}
		else{
			localMusic->duration = localMusic->music->getDuration();
		}

		localMusic->queued = true;
		channels[id] = localMusic;

		channels[before]->next = id;

		return id;
	}
}

void MusicManager::stop(const unsigned long& id, const bool& force){
	if(channels.count(id) == 0){
		return;
	}

	mm::Music* m = channels[id];

	if(force || !m->fadeOut){
		m->music->stop();
	}
	else{
		m->duration = clock.getElapsedTime() - m->start + FADE_DURATION;
	}
	if(m->queued){
		std::map<unsigned long, mm::Music*>::iterator itr = channels.begin();
		while(itr != channels.end()){
			std::pair<unsigned long, mm::Music*> mp = (*itr);
			if(mp.second == m){
				delete (*itr).second;
				itr = channels.erase(itr);
			}
			else{
				++itr;
			}
		}
	}
}

bool MusicManager::archiveMusic(){
	return archiveMusicFromDir(c::musicDir);
}

bool MusicManager::archiveMusicFromDir(File& dir){
	if(!dir.exists() || !dir.isDirectory()){
		return false;
	}

	bool success = true;

	for(File file : dir.listFiles()){
		if(file.isDirectory()){
			success = !archiveMusicFromDir(file) ? false : success;
			continue;
		}

		if(!Music().openFromFile(file.path())){
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