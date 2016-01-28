#pragma once

#include <vector>

#include "Entity.h"
#include "SFMLI.h"

static enum Layer{
	BACKGROUND,
	FOREGROUND
};

class World{
public:
	World();
	~World();

	void tick(sf::RenderWindow* window);

	void render(sf::RenderWindow* window);
private:
	sf::Clock clock;
	sf::Time lastTime;

	std::vector<Entity*> entities;

	void cleanAll(const bool& all);
};