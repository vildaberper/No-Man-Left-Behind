#include "Injured.h"

Injured::Injured(){
	
}

Injured::~Injured(){

}

void Injured::initialize(Manager* manager, const std::string& animation, const Injure& injure, const size_t& progress){
	setAnimationType(STATES, 5);
	applyAnimation(manager, id = animation);
	Injured::injure = injure;
	Injured::progress = progress;
	updateAnimation();
	cb.shouldCollide = true;
	cb.renderOffset = 0.9f;
	cb.offset = Vector(0.2f, 0.3f);
	cb.size = Vector(0.6f, 0.6f);
}

void Injured::updateAnimation(){
	currentAnimation = nextAnimation = state(INJURED_STATES - 1 + progress - requirements.at(injure).size());
	if(isHealed()){
		if(voice){
			si::stopSound(currentVoice);
		}
	}
	else{
		if(!voice){
			//voice = true;
			//currentVoice = si::playSound("voice." + id, true);
		}
	}
}

bool Injured::isHealed(){
	return progress >= requirements.at(injure).size();
}

bool Injured::use(ItemStack& is){
	if(is.amount > 0 && !isHealed() && is.item.type == requirements.at(injure).at(progress)){
		progress++;
		updateAnimation();
		is.amount--;
		si::playSound("resources." + resourceToString(is.item.type));
		return true;
	}
	return false;
}