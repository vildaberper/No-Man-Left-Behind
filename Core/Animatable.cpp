#include "Animatable.h"

Animatable::Animatable(){

}

Animatable::~Animatable(){

}

void Animatable::applyAnimation(Manager* m, const std::string& animation){
	switch(animatableType){
	case DIRECTIONAL:
		applyAnimation(m, animation, "leftidle");
		applyAnimation(m, animation, "left");
		applyAnimation(m, animation, "rightidle");
		applyAnimation(m, animation, "right");
		applyAnimation(m, animation, "upidle");
		applyAnimation(m, animation, "up");
		applyAnimation(m, animation, "downidle");
		applyAnimation(m, animation, "down");
		break;
	case STATES:
		for(unsigned int i = 0; i < numStates; i++){
			applyAnimation(m, animation, "state" + std::to_string(i));
		}
		setAnimation("state0");
		break;
	}
}

bool Animatable::hasAnimation(const std::string& animation){
	return animations.count(animation) != 0;
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
			setAnimation("left");
			break;
		case XP:
			setAnimation("right");
			break;
		case YN:
			setAnimation("up");
			break;
		case YP:
			setAnimation("down");
			break;
		case ZERO:
			if(currentAnimation == "left"){
				setAnimation("leftidle");
			}
			else if(currentAnimation == "right"){
				setAnimation("rightidle");
			}
			else if(currentAnimation == "up"){
				setAnimation("upidle");
			}
			else if(currentAnimation == "down"){
				setAnimation("downidle");
			}
			break;
		default:
			setAnimation("downidle");
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

void Animatable::applyAnimation(Manager* m, const std::string& category, const std::string& name){
	std::string full = category + "." + name;

	if(m->animationManager->hasAnimation(full)){
		animations[name] = m->animationManager->getAnimation(full);
	}
}

void Animatable::setAnimation(const std::string& animation){
	if(hasAnimation(animation)){
		currentAnimation = nextAnimation = animation;
	}
}