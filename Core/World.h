#pragma once

#include <vector>

#include "Drawable.h"
#include "Manager.h"
#include "Entity.h"
#include "SFMLI.h"
#include "GI.h"

// interv(x0, x1) + interv(y0, y1) !! NOT EUCLIDEAN DISTANCE !!
static const float MAX_COLLISION_DISTANCE = 3000.0f;

static const unsigned int MAX_SWAPS_PER_FRAME = 100;

enum Layer{
	LAYER0 = 0,
	LAYER1 = 1,
	LAYER2 = 2,
	LAYER3 = 3,
	LAYER4 = 4,
	NUM_LAYERS = 5
};

static const Layer nextLayer(const Layer& current){
	switch(current){
	case LAYER0:
		return LAYER1;
		break;
	case LAYER1:
		return LAYER2;
		break;
	case LAYER2:
		return LAYER3;
		break;
	case LAYER3:
		return LAYER4;
		break;
	case LAYER4:
		return LAYER0;
		break;
	}
	return LAYER0;
}

static const Layer previousLayer(const Layer& current){
	switch(current){
	case LAYER0:
		return LAYER4;
		break;
	case LAYER1:
		return LAYER0;
		break;
	case LAYER2:
		return LAYER1;
		break;
	case LAYER3:
		return LAYER2;
		break;
	case LAYER4:
		return LAYER3;
		break;
	}
	return LAYER0;
}

static const Layer parseLayer(const std::string& s){
	if(s == "LAYER0"){
		return LAYER0;
	}
	else if(s == "LAYER1"){
		return LAYER1;
	}
	else if(s == "LAYER2"){
		return LAYER2;
	}
	else if(s == "LAYER3"){
		return LAYER3;
	}
	else if(s == "LAYER4"){
		return LAYER4;
	}
	return LAYER0;
}

static const std::string layerToString(const Layer& layer){
	if(layer == LAYER0){
		return "LAYER0";
	}
	else if(layer == LAYER1){
		return "LAYER1";
	}
	else if(layer == LAYER2){
		return "LAYER2";
	}
	else if(layer == LAYER3){
		return "LAYER3";
	}
	else if(layer == LAYER4){
		return "LAYER4";
	}
	return "LAYER0";
}

class Target{
public:
	drawable::Drawable* drawable;
	Layer layer;
	float dx;
	float dy;

	Target(drawable::Drawable* drawable, Layer layer, float dx, float dy){
		Target::drawable = drawable;
		Target::layer = layer;
		Target::dx = dx;
		Target::dy = dy;
	}
	~Target(){

	}
};

class World {
public:
	World(Manager* manager);
	~World();

	void tick();

	const void renderBackground();
	const void render();
	const void render(drawable::Drawable* relative);

	void setPaused(const bool& paused);
	bool isPaused();

	void setTimeScale(const float& timeScale);
	float getTimeScale();

	const sf::Time time();

	const float dt();

	bool isOrdering();

	void orderDrawables(const Layer& layer);

	void insertDrawableAt(drawable::Drawable* drawable, const Layer& layer, const size_t& index);

	size_t binarySearchRenderOffset(drawable::Drawable* drawable, const Layer& layer);

	void orderDrawable(const size_t& index, const Layer& layer);

	void addDrawable(drawable::Drawable* drawable, const Layer& layer);

	Target* drawableAt(const float& x, const float& y, const Layer& layer);

	void save(File& f);

	void load(File& f);

	Manager* manager;

	std::string backgroundName;
	sf::Texture* background;

	std::map<Layer, std::vector<drawable::Drawable*>> drawables;
	std::vector<drawable::Drawable*> collidables;
	std::vector<Entity*> entities;

	Layer swapLayer;
	std::map<Layer, bool> doSwap;
	std::map<Layer, size_t> swapPosition;
private:
	bool firstTick = true;
	bool paused = false;
	sf::Clock clock;
	sf::Time t;
	sf::Time dt_;
	float timeScale = 1.0f;

	void cleanAll(const bool& all);
};