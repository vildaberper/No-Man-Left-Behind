#include "MusicManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

// 0-100, must be over 0.f
static float MASTER_VOLUME(1.0f);

enum fadingState { not, between, out };
fadingState fade;

MusicManager::MusicManager(){
}


MusicManager::~MusicManager(){
}

bool MusicManager::initialize(RenderWindow* window){
	sf::Clock cl;

	fade = not;

	logger::timing("Music loaded in " + to_string(cl.getElapsedTime().asSeconds()) + " seconds");
}

bool MusicManager::finalize(RenderWindow* window){
	return true;
}

void MusicManager::tick(RenderWindow* window, const Time& time, const float& dt){

}

bool MusicManager::play(const string& category, const string& name){
	return true;
}

bool MusicManager::fadeTo(const string& category, const string& name){
	return true;
}

bool MusicManager::fadeOut(){

	return true;
}