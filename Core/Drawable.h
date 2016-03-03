#pragma once

#include <vector>
#include <map>

#include "CollisionManager.h"
#include "CoreSprite.h"
#include "Entity.h"
#include "SFMLI.h"

namespace drawable{
	class Animation{
	public:
		std::vector<std::string> textures;
		std::vector<CoreSprite*> sprites;
		sf::Time timing;

		Animation(){
			
		}

		~Animation(){
			for(CoreSprite* cs : sprites){
				delete cs;
			}
			sprites.clear();
		}

		Animation* clone(){
			Animation* a = new Animation();

			a->textures = textures;
			for(CoreSprite* cs : sprites){
				a->sprites.push_back(cs->clone());
			}
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
		bool viewRelative = false;
		bool scaleRelative = true;

		Drawable();
		virtual ~Drawable();

		virtual void move(const float& dt);

		CoreSprite* getSprite(const sf::Time& time);

		sf::FloatRect bounds(const sf::Time& time);

		bool collidesWith(Drawable* d, const sf::Time& time, const Vector& position);
		bool collidesWith(Drawable* d, const sf::Time& time);

		void calcRenderOffset();

		float renderOffset();

		std::string reference = "";
		CollisionBox cb;

		float renderOffset_;
		bool movedY = false;
		bool updateOrder = false;
	};
}