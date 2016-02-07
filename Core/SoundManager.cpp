#include "SoundManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

// 0-100, must be over 0.f
static float VOLUME_TARGET(100.f);

SoundManager::SoundManager(){

}

SoundManager::~SoundManager(){

}

bool SoundManager::initialize(){
	sf::Clock cl;

	// Loading sounds
	if (loadSound()){
		logger::timing("Sounds loaded in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("SoundManager failed to initialize");
		return false;
	}

}

bool SoundManager::finalize(){
	// Empty containers
	for (SoundBuffer* sb : sounds){
		delete sb;
	}
	for (Sound* s : channels){
		delete s;
	}
	sounds.clear();
	channels.clear();
	soundBoard.clear();

	return true;
}

bool SoundManager::tick(const Time& time, const float& dt){
	
	return true;
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
		channels.back->setBuffer(*soundBoard[category][name]);
		channels.back->setVolume(VOLUME_TARGET);
		channels.back->play();
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

bool SoundManager::loadSound(){
	return loadSoundFromDir(c::soundDir);
}

bool SoundManager::loadSoundFromDir(File& dir){
	if (!dir.exists() || !dir.isDirectory()){
		return false;
	}

	bool success = true;
	for (File dirFile : dir.listFiles()){
		SoundBuffer* localBuffer = new SoundBuffer();

		if (!localBuffer->loadFromFile("TODO.ogg")){
			delete localBuffer;
			logger::warning("Failed to load sound: +TODO+ ");
			success = false;
			continue;
		}

		// TODO: fix dynamic naming
		String category = "TODO: category";
		String name = "TODO: name";

		// Adding sound to the map and set
		soundBoard[category][name] = localBuffer;
		sounds.insert(localBuffer);
	}

	// All sound loaded
	return success;
}