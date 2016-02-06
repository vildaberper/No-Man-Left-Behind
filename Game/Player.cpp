#include "Player.h"

Player::Player(){

}

Player::~Player(){

}

void Player::initialize(Manager* m){
	drawable::Animation* left = new drawable::Animation();
	left->timing = sf::milliseconds(100);
	for (int i = 0; i < 8; i++){
		std::string s = "animationleft." + std::to_string(i);
		left->textures.push_back(s);
		left->sprites.push_back(m->spriteManager->getSprite(s));
	}
	animations["left"] = left;
	drawable::Animation* idleleft = new drawable::Animation();
	idleleft->timing = sf::milliseconds(100);
	idleleft->textures.push_back("animationleft.0");
	idleleft->sprites.push_back(m->spriteManager->getSprite("animationleft.0"));
	animations["idleleft"] = idleleft;

	drawable::Animation* right = new drawable::Animation();
	right->timing = sf::milliseconds(100);
	for (int i = 0; i < 8; i++){
		std::string s = "animationright." + std::to_string(i);
		right->textures.push_back(s);
		right->sprites.push_back(m->spriteManager->getSprite(s));
	}
	animations["right"] = right;
	drawable::Animation* idleright = new drawable::Animation();
	idleright->timing = sf::milliseconds(100);
	idleright->textures.push_back("animationright.0");
	idleright->sprites.push_back(m->spriteManager->getSprite("animationright.0"));
	animations["idleright"] = idleright;

	currentAnimation = nextAnimation = "idleleft";
	scale = 1.8f;
}