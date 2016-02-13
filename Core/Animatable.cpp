#include "Animatable.h"

Animatable::Animatable(){

}

Animatable::~Animatable(){

}

void Animatable::apply(Manager* m, const std::string& a){
	animations["leftidle"] = m->animationManager->getAnimation(a + ".leftidle");
	animations["left"] = m->animationManager->getAnimation(a + ".left");
	animations["rightidle"] = m->animationManager->getAnimation(a + ".rightidle");
	animations["right"] = m->animationManager->getAnimation(a + ".right");
	animations["upidle"] = m->animationManager->getAnimation(a + ".upidle");
	animations["up"] = m->animationManager->getAnimation(a + ".up");
	animations["downidle"] = m->animationManager->getAnimation(a + ".downidle");
	animations["down"] = m->animationManager->getAnimation(a + ".down");
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