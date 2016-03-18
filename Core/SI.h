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
	unsigned long queueMusic(const unsigned long& before, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);
	unsigned long queueMusic(const unsigned long& before, const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop);
	void stopMusic(const unsigned long& id, const bool& force = false);
	//Sound
	unsigned long playSound_(const Entity* source, const std::string& category, const std::string& name, const bool& loop = false);
	unsigned long playSound(const Entity* source, const std::string& name, const bool& loop = false);
	unsigned long playSoundV(const Entity* source, const std::string& name, const float& volume, const bool& loop = false);
	unsigned long playRandomSound(const Entity* source, const std::string& category, const bool& loop = false);
	unsigned long playRandomSoundV(const Entity* source, const std::string& category, const float& volume, const bool& loop = false);
	void setSoundV(const unsigned long& id, const float& volume);
	void stopSound(const unsigned long& id);
}