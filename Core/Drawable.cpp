#include "GI.h"
#include "Drawable.h"

using namespace drawable;

Drawable::Drawable(){

}


Drawable::~Drawable(){

}

void Drawable::move(const float& dt){
	if(velocity.direction() != ZERO){
		hasRenderOffset_ = false;
	}
	Entity::move(dt);
}

sf::Sprite* Drawable::getSprite(const sf::Time& time){
	Animation* a;
	sf::Sprite* s;
	if(animations.count(currentAnimation) == 0){
		a = animations.begin()->second;
	}
	else{
		a = animations[currentAnimation];
	}
	if(time.asMicroseconds() == 0){
		s = a->sprites[0];
	}
	else if(a->sprites.size() == 1 || a->timing.asMilliseconds() == 0){
		if(currentAnimation == nextAnimation){
			s = a->sprites[0];
		}
		else{
			currentAnimation = nextAnimation;
			startTime = time;
			return getSprite(time);
		}
	}
	else{
		sf::Time elapsed = time - startTime;
		sf::Time maxDuration = sf::milliseconds(a->timing.asMilliseconds() * (a->sprites.size()));
		size_t index = size_t(floor((a->sprites.size()) * (elapsed / maxDuration)));

		if(index > a->sprites.size() - 1){
			currentAnimation = nextAnimation;
			startTime = time;
			return getSprite(time);
		}
		s = a->sprites[index];
	}

	// +0.375f because bleeding. what
	if(viewRelative){
		s->setPosition(
			round((position.x) * gi::dx()) + 0.375f,
			round((position.y) * gi::dy()) + 0.375f
			);
	}
	else{
		s->setPosition(
			round((position.x - gi::cameraX + gi::WIDTH / 2) * gi::dx()) + 0.375f,
			round((position.y - gi::cameraY + gi::HEIGHT / 2) * gi::dy()) + 0.375f
			);
	}
	s->scale(
		(1.0f / s->getScale().x) * scale * gi::dx(),
		(1.0f / s->getScale().y) * scale * gi::dy()
		);
	if(highlight){
		s->setColor(sf::Color(255, 255, 255, int(205 + 50 * sin(time.asMilliseconds() / 100.0f))));
	}
	else{
		s->setColor(sf::Color(255, 255, 255, 255));
	}
	renderOffset_ = position.y + s->getLocalBounds().height * cb.renderOffset * scale;
	hasRenderOffset_ = true;
	return s;
}

sf::FloatRect Drawable::bounds(const sf::Time& time){
	sf::FloatRect fr = getSprite(time)->getLocalBounds();

	fr.width *= scale;
	fr.height *= scale;
	fr.left += fr.width * cb.offset.x + position.x;
	fr.top += fr.height * cb.offset.y + position.y;
	fr.width *= cb.size.x;
	fr.height *= cb.size.y;

	return fr;
}

bool Drawable::collidesWith(Drawable* d, const sf::Time& time, const Vector& position){
	sf::FloatRect fr = bounds(time);
	fr.left += position.x - Drawable::position.x;
	fr.top += position.y - Drawable::position.y;
	return cb.shouldCollide && d->cb.shouldCollide && fr.intersects(d->bounds(time));
}
bool Drawable::collidesWith(Drawable* d, const sf::Time& time) {
	return cb.shouldCollide && d->cb.shouldCollide && bounds(time).intersects(d->bounds(time));
}

float Drawable::renderOffset(){
	if(!hasRenderOffset_){
		getSprite(sf::milliseconds(0));
	}
	return renderOffset_;
}