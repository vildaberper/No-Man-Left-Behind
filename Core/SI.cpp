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
	void stopMusic(const unsigned long& id, const bool& force){
		musicManager->stop(id, force);
	}
	//Sound
	unsigned long playSound_(const std::string& category, const std::string& name, const bool& loop){
		return soundManager->play(category, name, loop);
	}
	unsigned long playSound(const std::string& name, const bool& loop){
		return soundManager->play(name, loop);
	}
	void stopSound(const unsigned long& id){
		soundManager->stop(id);
	}
}