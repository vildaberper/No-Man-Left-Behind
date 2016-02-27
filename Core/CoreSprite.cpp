#include "CoreSprite.h"

CoreSprite::CoreSprite(sf::Sprite* sprite, int w, int h){
	sprite_ = sprite;
	w_ = w;
	h_ = h;
}
CoreSprite::~CoreSprite(){
	delete sprite_;
}

sf::Sprite* CoreSprite::sprite(){
	return sprite_;
}

int CoreSprite::w(){
	return w_;
}
int CoreSprite::h(){
	return h_;
}