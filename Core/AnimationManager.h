#pragma once

#include "Drawable.h"
#include "Constants.h"
#include "Configuration.h"
#include "SpriteManager.h"

class AnimationManager{
public:
	AnimationManager();
	~AnimationManager();

	void initialize(sf::RenderWindow* window, SpriteManager* sm);

	void finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	drawable::Animation* getAnimation(const std::string& name);
private:
	std::map<std::string, drawable::Animation*> animations;
	drawable::Animation* undefined;

	void loadFromDir(File& dir, SpriteManager* sm);
};