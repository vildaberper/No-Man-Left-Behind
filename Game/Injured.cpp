#include "Injured.h"

Injured::Injured(){
	customJournal = new Journal();
}

Injured::~Injured(){
	delete customJournal;
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
	customJournal->lines.clear();
	for(size_t i = 0; i < journal->lines.size(); i++){
		std::string line = journal->lines[i];
		size_t index;

		if((index = line.find("%state%")) != std::string::npos){
			std::string state;
			switch(INJURED_STATES - 1 + progress - journal->requirements.size()){
			case 0:
				state = "Dead";
				break;
			case 1:
				state = "Critical";
				break;
			case 2:
				state = "Serious";
				break;
			case 3:
				state = "Fair";
				break;
			case 4:
				state = "Healed";
				break;
			}
			line.replace(index, 7, state);
		}
		customJournal->lines.push_back(line);
	}
	for(size_t i = 0; i < progress; i++){
		customJournal->lines.push_back("");
		customJournal->lines.push_back("Treated with " + resourceToString(journal->requirements[i]) + ".");
	}
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