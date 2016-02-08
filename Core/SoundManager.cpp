#include "SoundManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

// 0-100, must be over 0.f
static float VOLUME_TARGET(100.f);

bool playing = false;

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
}

bool SoundManager::initialize(RenderWindow* window){
	sf::Clock cl;
	// Members
	playing = true;

	// Loading sounds
	if (loadSounds()){
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
	for (SoundBuffer* sb : sounds){
		delete sb;
	}
	for (Sound* s : channels){
		delete s;
	}
	sounds.clear();
	channels.clear();
	soundBoard.clear(); // Dose this work?

	return true;
}

void SoundManager::tick(RenderWindow* window, const Time& time, const float& dt){
	// Cleaning stoped sounds
	for (size_t i = 0; i < channels.size(); i++){
		if (channels[i]->getStatus() == Sound::Stopped ){
			delete channels[i];
			channels.erase(channels.begin() + (i--));
		}
	}
	// If game is paused
	if (dt <= 0.0f){
		if (playing){
			for (Sound* s : channels){
				s->pause();
			}
			playing = false;
		}
	}
	else{
		if (!playing){
			for (Sound* s : channels){
				s->play();
			}
			playing = true;
		}
	}
}

bool SoundManager::play(const string& category, const string& name){
	bool placing = true;
	for (Sound* s : channels){
		if (placing && s->getStatus() == Sound::Stopped){
			s->setBuffer(*soundBoard[category][name]);
			s->setVolume(VOLUME_TARGET);
			s->play();
			placing = false;
		}
	}
	// can add a cap to how many sounds can play at once
	if (placing){
		channels.push_back(new Sound());
		channels.back()->setBuffer(*soundBoard[category][name]);
		channels.back()->setVolume(VOLUME_TARGET);
		channels.back()->play();
		placing = false;
	}
	//TODO: positional sound
	if (placing){
		logger::fatal("Failed to find a place for " + category + " " + name + ".");
		return false;
	}
	else{
		return true;
	}
}

bool SoundManager::loadSounds(){
	return loadSoundsFromDir(c::soundDir);
}

bool SoundManager::loadSoundsFromDir(File& dir){
	if (!dir.exists() || !dir.isDirectory()){
		return false;
	}
	bool success = true;
	for (File file : dir.listFiles()){
		if (file.isDirectory()){
			success = !loadSoundsFromDir(file) ? false : success;
		}

		SoundBuffer* localBuffer = new SoundBuffer();

		if (!localBuffer->loadFromFile(file.path())){
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