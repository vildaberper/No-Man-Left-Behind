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
		round((position.x - gi::cameraX + gi::WIDTH / 2) * gi::dx()) + 0.375f,
		round((position.y - gi::cameraY + gi::HEIGHT / 2) * gi::dy()) + 0.375f
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

sf::FloatRect Drawable::bounds(const sf::Time& time){
	sf::FloatRect fr = getSprite(time)->getLocalBounds();

	fr.width *= scale;
	fr.height *= scale;
	fr.left += fr.width * offset.x + position.x;
	fr.top += fr.height * offset.y + position.y;
	fr.width *= size.x;
	fr.height *= size.y;

	return fr;
}

bool Drawable::collidesWith(Drawable* d, const sf::Time& time, const Vector& position){
	sf::FloatRect fr = bounds(time);
	fr.left += position.x - Drawable::position.x;
	fr.top += position.y - Drawable::position.y;
	return shouldCollide && d->shouldCollide && fr.intersects(d->bounds(time));
}
bool Drawable::collidesWith(Drawable* d, const sf::Time& time) {
	return shouldCollide && d->shouldCollide && bounds(time).intersects(d->bounds(time));
}