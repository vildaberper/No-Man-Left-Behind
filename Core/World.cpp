#include "World.h"

World::World(){

}

World::~World(){

}

void World::tick(sf::RenderWindow* window){
	sf::Time time = clock.getElapsedTime();
	dt_ = (time - lastTime).asSeconds();

	for (Entity* e : entities){
		e->tick(time, dt_);
	}

	lastTime = time;
}

void World::render(sf::RenderWindow* window){
	sf::Time time = lastTime;

	// GI -> render all drawables in order
}

const sf::Time World::time(){
	return lastTime;
}

const float World::dt(){
	return dt_;
}

void World::cleanAll(const bool& all){
	/*drawable.erase(
		remove_if(drawables.begin(), drawables.end(),
		[](const Drawable* e) { all || return !((Entity*)e)->isAlive(); }),
		drawables.end());*/

	for (size_t i = 0; i < entities.size(); i++){
		if (all || !entities[i]->isAlive()){
			delete entities[i];
			entities.erase(entities.begin() + (i--));
		}
	}
}