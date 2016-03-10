#include "SI.h"

namespace si{
	SoundManager* soundManager;
	MusicManager* musicManager;

	bool initalize(SoundManager*& sm, MusicManager*& mm){
		soundManager = sm;
		musicManager = mm;

		return true;
	}

	bool finalize(){
		return true;
	}

	//Music
	unsigned long playMusic(const std::string& name){
		return musicManager->play(name);
	}
	unsigned long playMusic(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
		return musicManager->play(name, fadeIn, fadeOut, loop);
	}
	unsigned long playMusic(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
		return musicManager->play(category, name, fadeIn, fadeOut, loop);
	}
	unsigned long queueMusic(const unsigned long& before, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
		return musicManager->queue(before, name, fadeIn, fadeOut, loop);
	}
	unsigned long queueMusic(const unsigned long& before, const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
		return musicManager->queue(before, category, name, fadeIn, fadeOut, loop);
	}
	void stopMusic(const unsigned long& id, const bool& force){
		musicManager->stop(id, force);
	}
	//Sound
	unsigned long playSound_(const Entity* source, const std::string& category, const std::string& name, const bool& loop){
		return soundManager->play(source, category, name, loop);
	}
	unsigned long playSound(const Entity* source, const std::string& name, const bool& loop){
		return soundManager->play(source, name, loop);
	}
	unsigned long playRandomSound(const Entity* source, const std::string& category, const bool& loop){
		return soundManager->playRandom(source, category, loop);
	}
	void stopSound(const unsigned long& id){
		soundManager->stop(id);
	}
}