#include "CollisionManager.h"

#include "Configuration.h"

using namespace std;

CollisionManager::CollisionManager(){
	
}

CollisionManager::~CollisionManager(){

}

bool CollisionManager::initialize(sf::RenderWindow* window){
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
			collisionBoxes[name] = cb;
		}
	}
	return true;
}
bool CollisionManager::finalize(sf::RenderWindow* window){
	save(c::collisionBoxFile);
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
	Configuration c;
	for(auto &ent : collisionBoxes){
		c.set(ent.first + ".shouldCollide", ent.second->shouldCollide);
		c.set(ent.first + ".offset", ent.second->offset.fv());
		c.set(ent.first + ".size", ent.second->size.fv());
	}
	return c.save(file);
}