#include "Animatable.h"

Animatable::Animatable(){

}

Animatable::~Animatable(){

}

void Animatable::applyAnimation(Manager* m, const std::string& animation){
	switch(animatableType){
	case DIRECTIONAL:
		animations["leftidle"] = m->animationManager->getAnimation(animation + ".leftidle");
		animations["left"] = m->animationManager->getAnimation(animation + ".left");
		animations["rightidle"] = m->animationManager->getAnimation(animation + ".rightidle");
		animations["right"] = m->animationManager->getAnimation(animation + ".right");
		animations["upidle"] = m->animationManager->getAnimation(animation + ".upidle");
		animations["up"] = m->animationManager->getAnimation(animation + ".up");
		animations["downidle"] = m->animationManager->getAnimation(animation + ".downidle");
		animations["down"] = m->animationManager->getAnimation(animation + ".down");
		break;
	case STATES:
		for(unsigned int i = 0; i < numStates; i++){
			animations["state" + std::to_string(i)] = m->animationManager->getAnimation(animation + "." + state(i));
		}
		currentAnimation = nextAnimation = "state0";
		break;
	}
}

std::string Animatable::state(const unsigned int& state){
	unsigned int s = state;

	if(s >= numStates){
		s = numStates - 1;
	}

	return "state" + std::to_string(s);
}

void Animatable::move(const float& dt){
	switch(animatableType){
	case DIRECTIONAL:
		switch(velocity.direction()){
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
			if(currentAnimation == "left"){
				currentAnimation = nextAnimation = "leftidle";
			}
			else if(currentAnimation == "right"){
				currentAnimation = nextAnimation = "rightidle";
			}
			else if(currentAnimation == "up"){
				currentAnimation = nextAnimation = "upidle";
			}
			else if(currentAnimation == "down"){
				currentAnimation = nextAnimation = "downidle";
			}
			break;
		default:
			currentAnimation = nextAnimation = "downidle";
			break;
		}
		break;
	}
	drawable::Drawable::move(dt);
}

void Animatable::setAnimationType(const AnimatableType& animatableType, const unsigned int& numStates){
	Animatable::animatableType = animatableType;
	setNumStates(numStates);
}
AnimatableType Animatable::getAnimationType(){
	return animatableType;
}

void Animatable::setNumStates(const unsigned int& numStates){
	Animatable::numStates = numStates;
}
unsigned int Animatable::getNumStates(){
	return numStates;
}