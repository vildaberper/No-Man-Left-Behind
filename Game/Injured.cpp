#include "Injured.h"

Injured::Injured(){
	
}

Injured::~Injured(){

}

void Injured::initialize(Manager* manager, const std::string& animation, const Injure& injure, const size_t& progress){
	animatableType = STATES;
	numStates = 5;
	apply(manager, animation);
	Injured::injure = injure;
	Injured::progress = progress;
	updateAnimation();
	cb.shouldCollide = true;
	cb.renderOffset = 1.0f;
	cb.offset = Vector(0.2f, 0.3f);
	cb.size = Vector(0.6f, 0.6f);
}

void Injured::updateAnimation(){
	setNextState(INJURED_STATES - 1 + progress - requirements.at(injure).size());
}

bool Injured::isHealed(){
	return progress >= requirements.at(injure).size();
}