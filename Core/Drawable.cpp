#include "GI.h"
#include "Drawable.h"

using namespace drawable;

Drawable::Drawable(){

}


Drawable::~Drawable(){

}

sf::Sprite* Drawable::getSprite(const sf::Time& time){
	Animation* a;
	if (animations.count(currentAnimation) == 0){
		a = animations.begin()->second;
	}
	else{
		a = animations[currentAnimation];
	}
	sf::Time elapsed = time - startTime;
	sf::Time maxDuration = sf::milliseconds(a->timing.asMilliseconds() * (a->sprites.size()));
	size_t index = size_t(floor((a->sprites.size()) * (elapsed / maxDuration)));

	if (index > a->sprites.size() - 1){
		currentAnimation = nextAnimation;
		startTime = time;
		return getSprite(time);
	}

	sf::Sprite* s = a->sprites[index];
	// +0.375f because bleeding. what
	s->setPosition(
		round((position.x - gi::cameraX + gi::TARGET_WIDTH / 2) * gi::dx()) + 0.375f,
		round((position.y - gi::cameraY + gi::TARGET_HEIGHT / 2) * gi::dy()) + 0.375f
		);
	s->scale(1.0f / s->getScale().x, 1.0f / s->getScale().y);
	s->scale(scale * gi::dx(), scale * gi::dy());
	if (highlight){
		s->setColor(sf::Color(255, 255, 255, int(205 + 50 * sin(time.asMilliseconds() / 100.0f))));
	}
	else{
		s->setColor(sf::Color(255, 255, 255, 255));
	}
	return s;
}