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
	//Sound
	bool playSound(const std::string& name);
	bool playSound(const std::string& category, const std::string& name);
}