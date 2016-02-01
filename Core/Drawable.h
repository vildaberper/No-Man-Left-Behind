#pragma once

#include <vector>
#include <map>

#include "Entity.h"
#include "SFMLI.h"

namespace drawable{
	class Animation{
	public:
		std::vector<std::string> textures;
		std::vector<sf::Sprite*> sprites;
		sf::Time timing; // length of each frame in ms (default 17 for one frame in 60 fps)
	};

	class Drawable : public Entity{
	public:
		std::map<std::string, Animation> animations;
		std::string currentAnimation;
		std::string nextAnimation;
		sf::Time startTime;
		float scale = 1.0f;
		float rotation = 0.0f;
		bool highlight = false;

		Drawable();
		~Drawable();

		virtual sf::Sprite* getSprite(const sf::Time& time);
	};
}