#pragma once

#include <map>

#include "Constants.h"
#include "Vector.h"
#include "SFMLI.h"

class CollisionBox{
public:
	CollisionBox(){

	};
	~CollisionBox(){

	};

	bool shouldCollide = false;
	Vector offset = Vector(0.0f, 0.0f);
	Vector size = Vector(1.0f, 1.0f);
};

class CollisionManager{
public:
	CollisionManager();
	~CollisionManager();

	bool initialize(sf::RenderWindow* window);
	bool finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	CollisionBox getCollisionBox(const std::string& name) const;
	CollisionBox* getCollisionBoxReference(const std::string& name);
	bool createCollisionBox(const std::string& name);

	bool save(File& file);
private:
	std::map<std::string, CollisionBox*> collisionBoxes;

	CollisionBox undefined = CollisionBox();
};