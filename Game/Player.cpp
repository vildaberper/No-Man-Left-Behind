#include "Player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::initialize(Manager* m){
	apply(m, "player");

	currentAnimation = nextAnimation = "leftidle";
	scale = 1.8f;
}