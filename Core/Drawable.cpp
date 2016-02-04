#include "GI.h"
#include "Drawable.h"

using namespace drawable;

Drawable::Drawable(){

}


Drawable::~Drawable(){

}

sf::Sprite* Drawable::getSprite(const sf::Time& time){
	sf::Time elapsed = time - startTime;
	sf::Time maxDuration = sf::milliseconds(animations[currentAnimation]->timing.asMilliseconds() * (animations[currentAnimation]->sprites.size()));
	size_t index = size_t(floor((animations[currentAnimation]->sprites.size() - 1) * (elapsed / maxDuration)));

	if (index > animations[currentAnimation]->sprites.size() - 1){
		currentAnimation = nextAnimation;
		startTime = time;
		return getSprite(time);
	}

	sf::Sprite* s = animations[currentAnimation]->sprites[index];
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