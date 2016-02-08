#include "MusicManager.h"

MusicManager::MusicManager(){

}

MusicManager::~MusicManager(){

}

bool MusicManager::initialize(sf::RenderWindow* window){
	return true;
}
bool MusicManager::finalize(sf::RenderWindow* window){
	return true;
}

void MusicManager::tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){

}

unsigned long MusicManager::play(const std::string& name){
	return 0;
}
unsigned long MusicManager::play(const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	return 0;
}
unsigned long MusicManager::play(const std::string& category, const std::string& name, const bool& fadeIn, const bool& fadeOut, const bool& loop){
	return 0;
}

void MusicManager::stop(const unsigned long& id, const bool& force){

}