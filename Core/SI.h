#pragma once

#include "MusicManager.h"
#include "SoundManager.h"

namespace si{
	extern SoundManager* soundManager;
	extern MusicManager* musicManager;

	// Initialize \ Finalize
	bool initalize(SoundManager*& sm, MusicManager*& mm);

	bool finalize();

	//Music
	unsigned long playMusic(const std::string& name);
	unsigned long playMusic(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);
	unsigned long playMusic(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);
	void stopMusic(const unsigned long& id, const bool& force);
	//Sound
	unsigned long playSound_(const std::string& category, const std::string& name, const bool& loop = false);
	unsigned long playSound(const std::string& name, const bool& loop = false);
	void stopSound(const unsigned long& id);
}