#pragma once

#include <vector>

#include "Drawable.h"
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

	const sf::Time time();

	const float dt();
private:
	sf::Clock clock;
	sf::Time lastTime;
	float dt_;

	std::vector<Entity*> entities;

	std::map<Layer, Drawable*> drawables;

	void cleanAll(const bool& all);
};