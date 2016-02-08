#include "Player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::initialize(Manager* m){
	animations["leftidle"] = m->animationManager->getAnimation("player.leftidle");
	animations["left"] = m->animationManager->getAnimation("player.left");
	animations["rightidle"] = m->animationManager->getAnimation("player.rightidle");
	animations["right"] = m->animationManager->getAnimation("player.right");

	currentAnimation = nextAnimation = "leftidle";
	scale = 1.8f;
}