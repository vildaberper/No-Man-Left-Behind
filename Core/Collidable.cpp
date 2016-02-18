#include "Collidable.h"

using namespace sf;

Collidable::Collidable(){

}

Collidable::~Collidable(){

}

FloatRect Collidable::bounds(const Time& time){
	FloatRect fr = getSprite(time)->getGlobalBounds();

	fr.left += fr.width * offset.x;
	fr.top += fr.height * offset.y;
	fr.width = fr.width * size.x;
	fr.height = fr.height * size.y;

	return fr;
}

bool Collidable::collidesWith(Collidable* c, const Time& time) {
	return bounds(time).intersects(c->bounds(time));
}