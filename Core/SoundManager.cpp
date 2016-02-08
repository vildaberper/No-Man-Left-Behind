#include "SoundManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

bool playing = false;

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
}

bool SoundManager::initialize(RenderWindow* window){
	sf::Clock cl;

	undefined = new SoundBuffer();
	File u = c::soundDir.child("undefined.ogg");
	if (!u.isFile() || !undefined->loadFromFile(u.path())){
		logger::warning("Undefined sound not found!");
	}
	sounds.insert(undefined);

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
	for (Sound* s : channels){
		delete s;
	}
	channels.clear();
	for (SoundBuffer* sb : sounds){
		delete sb;
	}
	sounds.clear();

	soundBoard.clear();

	return true;
}

void SoundManager::tick(RenderWindow* window, const Time& time, const float& dt){
	// Cleaning stoped sounds
	for (size_t i = 0; i < channels.size(); i++){
		if (channels[i]->getStatus() == Sound::Stopped){
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

bool SoundManager::play(const std::string& name){
	std::string::size_type index = name.find_first_of('.');

	return play(name.substr(0, index), name.substr(index + 1));
}
bool SoundManager::play(const string& category, const string& name){
	// can add a cap to how many sounds can play at once
	if(channels.size() > 100){
		logger::warning("All them sounds are playing");
		return false;
	}

	Sound* s = new Sound();
	if (soundBoard.count(category) == 0 || soundBoard[category].count(name) == 0){
		s->setBuffer(*undefined);
	}
	else{
		s->setBuffer(*soundBoard[category][name]);
	}
	s->setVolume(c::masterVolume * 100.0f);
	s->play();
	channels.push_back(s);
	return true;
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