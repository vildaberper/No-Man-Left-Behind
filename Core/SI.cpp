#include "SI.h"

namespace si {
	SoundManager* soundManager;
	MusicManager* musicManager;

	bool initalize(SoundManager*& sm, MusicManager*& mm) {
		soundManager = sm;
		musicManager = mm;

		return true;
	}

	bool finalize() {
		delete soundManager;
		delete musicManager;

		return true;
	}

	//Music
	unsigned long playMusic(const std::string& name) {
		return musicManager->play(name);
	}
	unsigned long playMusic(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop) {
		return musicManager->play(name, fadeIn, fadeOut, loop);
	}
	unsigned long playMusic(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop) {
		return musicManager->play(category, name, fadeIn, fadeOut, loop);
	}
	//Sound
	bool playSound(const std::string& name) {
		return soundManager->play(name);
	}
	bool playSound(const std::string& category, const std::string& name) {
		return soundManager->play(category, name);
	}
}
