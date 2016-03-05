#include "Injured.h"

Injured::Injured(){
	
}

Injured::~Injured(){

}

void Injured::initialize(Manager* manager, const std::string& animation, Journal* journal){
	setAnimationType(STATES, 5);
	applyAnimation(manager, id = animation);
	Injured::journal = journal;
	Injured::progress = progress;
	updateAnimation();
	cb.shouldCollide = true;
	cb.renderOffset = 0.9f;
	cb.offset = Vector(0.2f, 0.3f);
	cb.size = Vector(0.6f, 0.6f);
}

void Injured::updateAnimation(){
	setAnimation(state(INJURED_STATES - 1 + progress - journal->requirements.size()));
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
	return progress >= journal->requirements.size();
}

bool Injured::use(ItemStack& is){
	if(is.amount > 0 && !isHealed() && is.item.type == journal->requirements[progress]){
		progress++;
		updateAnimation();
		is.amount--;
		si::playSound("resources." + resourceToString(is.item.type));
		return true;
	}
	return false;
}