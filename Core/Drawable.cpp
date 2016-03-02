#include "GI.h"
#include "Drawable.h"

using namespace drawable;

Drawable::Drawable(){

}


Drawable::~Drawable(){
	for(auto &ent : animations){
		delete ent.second;
		ent.second = NULL;
	}
	animations.clear();
}

void Drawable::move(const float& dt){
	movedY = velocity.y != 0.0f;
	if(movedY){
		updateOrder = true;
	}
	Entity::move(dt);
}

CoreSprite* Drawable::getSprite(const sf::Time& time){
	Animation* a;
	CoreSprite* s;
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
		size_t index = size_t(floor((a->sprites.size() * (elapsed / maxDuration))));

		if(index > a->sprites.size() - 1){
			currentAnimation = nextAnimation;
			startTime = time;
			return getSprite(time);
		}
		s = a->sprites[index];
	}

	if(viewRelative){
		s->sprite()->setPosition(
			round((position.x) * gi::dx()) + 0.375f,
			round((position.y) * gi::dy()) + 0.375f
			);
	}
	else{
		s->sprite()->setPosition(
			gi::sx(position.x),
			gi::sy(position.y)
			);
	}
	if(highlight){
		int c = int(205 + 50 * sin(time.asMilliseconds() / 100.0f));
		s->sprite()->setColor(sf::Color(c, c, c, 255));
	}
	else{
		s->sprite()->setColor(sf::Color(255, 255, 255, 255));
	}
	if(scaleRelative){
		s->sprite()->scale(
			(1.0f / s->sprite()->getScale().x) * scale * gi::dx(),
			(1.0f / s->sprite()->getScale().y) * scale * gi::dy()
			);
	}
	else{
		s->sprite()->scale(
			(1.0f / s->sprite()->getScale().x) * scale,
			(1.0f / s->sprite()->getScale().y) * scale
			);
	}
	return s;
}

sf::FloatRect Drawable::bounds(const sf::Time& time){
	CoreSprite* cs = getSprite(time);

	return sf::FloatRect(
		position.x + cs->w() * scale * cb.offset.x,
		position.y + cs->h() * scale * cb.offset.y,
		cs->w() * scale * cb.size.x,
		cs->h() * scale * cb.size.y
		);
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

void Drawable::calcRenderOffset(){
	renderOffset_ = float(animations.begin()->second->sprites[0]->h()) * cb.renderOffset;
}

float Drawable::renderOffset(){
	return position.y + renderOffset_ * scale;
}