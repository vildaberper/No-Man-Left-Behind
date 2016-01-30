#include "World.h"

World::World(){
	drawables[LAYER0] = std::vector<drawable::Drawable*>();
	drawables[LAYER1] = std::vector<drawable::Drawable*>();
	drawables[LAYER2] = std::vector<drawable::Drawable*>();
	drawables[LAYER3] = std::vector<drawable::Drawable*>();
	drawables[LAYER4] = std::vector<drawable::Drawable*>();
	lastTime = clock.getElapsedTime();
}

World::~World(){

}

void World::tick(){
	sf::Time time = clock.getElapsedTime();
	dt_ = (time - lastTime).asSeconds();

	for (Entity* e : entities){
		e->tick(time, dt_);
	}

	lastTime = time;
	cleanAll(false);
}

const void World::render(){
	for (const auto &ent : drawables){
		for (drawable::Drawable* d : ent.second){
			gi::draw(d, lastTime);
		}
	}
}

const sf::Time World::time(){
	return lastTime;
}

const float World::dt(){
	return dt_;
}

void World::addDrawable(drawable::Drawable* drawable, const Layer& layer){
	entities.push_back((Entity*) drawable);
	drawables[layer].push_back(drawable);
}

void World::cleanAll(const bool& all){
	for (auto &ent : drawables){
		ent.second.erase(
			remove_if(ent.second.begin(), ent.second.end(),
			[all](const drawable::Drawable* e){ return all || !((Entity*)e)->isAlive(); }),
			ent.second.end());
	}

	for (size_t i = 0; i < entities.size(); i++){
		if (all || !entities[i]->isAlive()){
			delete entities[i];
			entities.erase(entities.begin() + (i--));
		}
	}
}