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
		sf::Time timing; // length of each frame in ms

		Animation* clone(){
			Animation* a = new Animation();

			a->textures = textures;
			a->sprites = sprites;
			a->timing = timing;

			return a;
		}
	};

	class Drawable : public Entity{
	public:
		std::map<std::string, Animation*> animations;
		std::string currentAnimation;
		std::string nextAnimation;
		sf::Time startTime;
		float scale = 1.0f;
		bool highlight = false;

		Drawable();
		~Drawable();

		sf::Sprite* getSprite(const sf::Time& time);

		sf::FloatRect bounds(const sf::Time& time);

		bool collidesWith(Drawable* d, const sf::Time& time, const Vector& position);
		bool collidesWith(Drawable* d, const sf::Time& time);

		Vector offset = Vector(0.0f, 0.0f);
		Vector size = Vector(1.0f, 1.0f);
		bool shouldCollide = false;
	};
}