#include "Player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::initialize(Manager* m){
	apply(m, "player");

	currentAnimation = nextAnimation = "leftidle";
	scale = 1.8f;
	shouldCollide = true;
	offset = Vector(0.4f, 0.8f);
	size = Vector(0.2f, 0.1f);
}