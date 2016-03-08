#include "Handbook.h"

using namespace handbook;

Page::Page(){
	
}

Page::~Page(){

}

Handbook::Handbook(Manager* m){
	setAnimationType(STATES, 4);
	applyAnimation(m, "handbook");
	viewRelative = true;
	currentAnimation = nextAnimation = CLOSED;
	position.x = gi::TARGET_WIDTH / 2.0f;
	position.y = gi::TARGET_HEIGHT / 2.0f;
	scale = 1.0f;

	CoreSprite* cs = getSprite(sf::microseconds(0));
	position.x -= cs->w() * scale / 2.0f;
	position.y -= cs->h() * scale / 2.0f;
}
Handbook::~Handbook(){

}

void Handbook::openHandbook(const sf::Time& time){
	if (!isClosed()){
		return;
	}
	currentAnimation = OPENING;
	nextAnimation = OPENED;
	startTime = time;
}
void Handbook::closeHandbook(const sf::Time& time){
	if (!isOpen()){
		return;
	}
	currentAnimation = CLOSING;
	nextAnimation = CLOSED;
	startTime = time;
}
bool Handbook::hasLeftPage(){

}
bool Handbook::hasRightPage(){

}
void Handbook::turnLeft(){

}
void Handbook::turnRight(){

}
bool Handbook::isOpen(){
	return currentAnimation == OPENED;
}
bool Handbook::isClosed(){
	return currentAnimation == CLOSED;
}

void Handbook::render(const sf::Time& time){
	CoreSprite* cs = getSprite(time);
	bool renderText = currentAnimation == OPENED;
	if (currentAnimation != CLOSED) {
		gi::draw(*cs);
	}
}
