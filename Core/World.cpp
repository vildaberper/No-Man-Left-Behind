#include "World.h"

World::World(){

}

World::~World(){

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