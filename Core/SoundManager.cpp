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
		logger::timing("Sounds loaded in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("SoundManager failed to initialize");
		return false;
	}
}

bool SoundManager::finalize(RenderWindow* window){
	// Empty containers
	for(auto &ent : channels){
		ent.second->stop();
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

void SoundManager::tick(RenderWindow* window, const Time& time, const float& dt){
	// Cleaning stopped sounds
	for(auto &ent : channels){
		if(ent.second->getStatus() == Sound::Stopped){
			delete ent.second;
			ent.second = NULL;
		}
	}
	for(std::map<unsigned long, Sound*>::iterator i = channels.begin(); i != channels.end(); ) {
		if(i->second == NULL) {
			channels.erase(i++);
		}
		else {
			++i;
		}
	}

	// If game is paused
	if(dt <= 0.0f){
		if(playing){
			for(auto &ent : channels){
				ent.second->pause();
			}
			playing = false;
		}
	}
	else{
		if(!playing){
			for(auto &ent : channels){
				ent.second->play();
			}
			playing = true;
		}
	}
}

unsigned long SoundManager::play(const std::string& name, const bool& loop){
	std::string::size_type index = name.find_first_of('.');

	return play(name.substr(0, index), name.substr(index + 1), loop);
}
unsigned long SoundManager::play(const string& category, const string& name, const bool& loop){
	unsigned long id = idTracker++;

	// can add a cap to how many sounds can play at once
	if(channels.size() > 100){
		logger::warning("All them sounds are playing");
		return false;
	}

	Sound* s = new Sound();
	if(soundBoard.count(category) == 0 || soundBoard[category].count(name) == 0){
		s->setBuffer(*undefined);
	}
	else{
		s->setBuffer(*soundBoard[category][name]);
	}
	s->setVolume(c::masterVolume * 100.0f);
	s->setLoop(loop);
	s->play();
	channels[id] = s;
	return id;
}

void SoundManager::stop(const unsigned long& id){
	if(channels.count(id) > 0){
		channels[id]->stop();
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