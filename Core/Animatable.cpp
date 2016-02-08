#include "Animatable.h"

Animatable::Animatable(){

}

Animatable::~Animatable(){

}

void Animatable::tick(const sf::Time& time, const float& dt){
	switch (velocity.direction()){
	case XN:
		currentAnimation = nextAnimation = "left";
		break;
	case XP:
		currentAnimation = nextAnimation = "right";
		break;
	case YN:
		currentAnimation = nextAnimation = "up";
		break;
	case YP:
		currentAnimation = nextAnimation = "down";
		break;
	case ZERO:
		if (currentAnimation == "left"){
			currentAnimation = nextAnimation = "leftidle";
		}
		else if (currentAnimation == "right"){
			currentAnimation = nextAnimation = "rightidle";
		}
		else if (currentAnimation == "up"){
			currentAnimation = nextAnimation = "upidle";
		}
		else if (currentAnimation == "down"){
			currentAnimation = nextAnimation = "downidle";
		}
		break;
	default:
		currentAnimation = nextAnimation = "downidle";
		break;
	}
	drawable::Drawable::tick(time, dt);
}