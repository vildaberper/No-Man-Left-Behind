#pragma once

#include <vector>

#include "Drawable.h"
#include "Entity.h"
#include "SFMLI.h"
#include "GI.h"

static enum Layer{
	LAYER0 = 0,
	LAYER1 = 1,
	LAYER2 = 2,
	LAYER3 = 3,
	LAYER4 = 4
};

class Target{
public:
	drawable::Drawable* drawable;
	Layer layer;

	Target(drawable::Drawable* drawable, Layer layer){
		Target::drawable = drawable;
		Target::layer = layer;
	}
};

class World{
public:
	World();
	~World();

	void tick();

	const void render();

	const sf::Time time();

	const float dt();

	void addDrawable(drawable::Drawable* drawable, const Layer& layer);

	Target* drawableAt(const float& x, const float& y){
		for (drawable::Drawable* d : drawables[LAYER4]){
			if (d->getSprite(lastTime)->getGlobalBounds().contains(sf::Vector2f(x, y))){
				return new Target(d, LAYER4);
			}
		}
		for (drawable::Drawable* d : drawables[LAYER3]){
			if (d->getSprite(lastTime)->getGlobalBounds().contains(sf::Vector2f(x, y))){
				return new Target(d, LAYER3);
			}
		}
		for (drawable::Drawable* d : drawables[LAYER2]){
			if (d->getSprite(lastTime)->getGlobalBounds().contains(sf::Vector2f(x, y))){
				return new Target(d, LAYER2);
			}
		}
		for (drawable::Drawable* d : drawables[LAYER1]){
			if (d->getSprite(lastTime)->getGlobalBounds().contains(sf::Vector2f(x, y))){
				return new Target(d, LAYER1);
			}
		}
		for (drawable::Drawable* d : drawables[LAYER0]){
			if (d->getSprite(lastTime)->getGlobalBounds().contains(sf::Vector2f(x, y))){
				return new Target(d, LAYER0);
			}
		}
		return NULL;
	}

	std::map<Layer, std::vector<drawable::Drawable*>> drawables;
private:
	sf::Clock clock;
	sf::Time lastTime;
	float dt_;

	std::vector<Entity*> entities;

	void cleanAll(const bool& all);
};