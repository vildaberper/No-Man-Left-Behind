#include "CoreSprite.h"

CoreSprite::CoreSprite(sf::Sprite* sprite, int w, int h){
	sprite_ = sprite;
	w_ = w;
	h_ = h;
}
CoreSprite::CoreSprite(sf::Texture* texture){
	sprite_ = new sf::Sprite(*texture);
	w_ = texture->getSize().x;
	h_ = texture->getSize().y;
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

CoreSprite* CoreSprite::clone(){
	return new CoreSprite(new sf::Sprite(*sprite_), w(), h());
}