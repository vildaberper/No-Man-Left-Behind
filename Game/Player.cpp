#include "Player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::initialize(Manager* m){
	apply(m, "player");

	currentAnimation = nextAnimation = "leftidle";
	scale = 1.0f;
	cb.shouldCollide = true;
	cb.offset = Vector(0.3f, 0.75f);
	cb.size = Vector(0.4f, 0.2f);
	cb.renderOffset = 0.84f;
}