#include "GI.h"
#include "Drawable.h"

using namespace drawable;

Drawable::Drawable(){

}


Drawable::~Drawable(){
	for(auto &ent : animations){
		delete ent.second;
		ent.second = nullptr;
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

const sf::Time fadeTime = sf::milliseconds(250);

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
			round((position.x) * gi::dxiz()) + 0.375f,
			round((position.y) * gi::dyiz()) + 0.375f
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
			(1.0f / s->sprite()->getScale().x) * scale * gi::dxiz(),
			(1.0f / s->sprite()->getScale().y) * scale * gi::dyiz()
			);
	}
	if(hideUnderCamera){
		if(sf::FloatRect(
			position.x + s->w() * scale * cb.offset.x,
			position.y + s->h() * scale * cb.offset.y,
			s->w() * scale * cb.size.x,
			s->h() * scale * cb.size.y
			).contains(gi::cameraTargetX + gi::relativeOffset.x, gi::cameraTargetY + gi::relativeOffset.y)){
			if(!hiddenUnderCamera){
				hiddenUnderCamera = true;
				hideTime = time;
			}
			float a = 1.0f - ((time - hideTime) / fadeTime);
			if(a < alpha){
				alpha = a < 0.0f ? 0.0f : a;
			}
		}
		else{
			if(hiddenUnderCamera){
				hiddenUnderCamera = false;
				hideTime = time;
			}
			float a = ((time - hideTime) / fadeTime);
			if(a > alpha){
				alpha = a > 1.0f ? 1.0f : a;
			}
		}
		sf::Color c = s->sprite()->getColor();
		c.a = sf::Uint8(c.a * alpha);
		s->sprite()->setColor(c);
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
	CoreSprite* cs = animations.begin()->second->sprites[0];

	size = Vector(cs->w() * scale, cs->h() * scale);
	renderOffset_ = float(cs->h()) * cb.renderOffset;
}

float Drawable::renderOffset(){
	return position.y + renderOffset_ * scale;
}