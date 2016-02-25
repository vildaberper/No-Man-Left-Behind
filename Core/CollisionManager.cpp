#include "CollisionManager.h"

#include "Configuration.h"

using namespace std;

CollisionManager::CollisionManager(){

}

CollisionManager::~CollisionManager(){

}

bool CollisionManager::initialize(sf::RenderWindow* window){
	sf::Clock cl;

	Configuration c;
	if(!c.load(c::collisionBoxFile)){
		logger::fatal("Failed to load collisionBox file: " + c::collisionBoxFile.parent().name() + "\\" + c::collisionBoxFile.name());
		return false;
	}
	for(string category : c.children("")){
		for(string name : c.children(category)){
			CollisionBox* cb = new CollisionBox();

			cb->shouldCollide = c.boolValue(name + ".shouldCollide");
			cb->offset = c.floatVector(name + ".offset");
			cb->size = c.floatVector(name + ".size");
			cb->renderOffset = c.floatValue(name + ".renderOffset");
			collisionBoxes[name] = cb;
		}
	}
	logger::timing("Collisionboxes initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	return true;
}
bool CollisionManager::finalize(sf::RenderWindow* window){
	for(auto &ent : collisionBoxes){
		delete ent.second;
	}
	collisionBoxes.clear();
	return true;
}

void CollisionManager::tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){

}

CollisionBox CollisionManager::getCollisionBox(const string& name) const{
	if(collisionBoxes.count(name) > 0){
		return *collisionBoxes.at(name);
	}
	return undefined;
}
CollisionBox* CollisionManager::getCollisionBoxReference(const std::string& name){
	createCollisionBox(name);
	return collisionBoxes[name];
}
bool CollisionManager::createCollisionBox(const std::string& name){
	if(collisionBoxes.count(name) > 0){
		return false;
	}
	collisionBoxes[name] = new CollisionBox();
	return true;
}

bool CollisionManager::save(File& file){
	sf::Clock cl;
	Configuration c;
	for(auto &ent : collisionBoxes){
		c.set(ent.first + ".shouldCollide", ent.second->shouldCollide);
		c.set(ent.first + ".offset", ent.second->offset.fv());
		c.set(ent.first + ".size", ent.second->size.fv());
		c.set(ent.first + ".renderOffset", ent.second->renderOffset);
	}
	bool success = c.save(file);

	logger::timing("Collisionboxes saved in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	logger::info("Collisionboxes saved: " + file.parent().name() + "\\" + file.name());
	return success;
}