#include "Injured.h"

namespace injured{
	sf::Time lastVoice;
}

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
	cb.offset = Vector(0.2f, 0.1f);
	cb.size = Vector(0.6f, 0.8f);
	nextVoice = sf::seconds(15.0f * random::random());
}

void Injured::tick(const sf::Time& time, const float& dt){
	if(isHealed()){
		si::stopSound(currentVoice);
	}
	else if(!isDead()){
		if(time - lastVoice > nextVoice){
			// Make sure two voices does not start at the same time
			if((time - injured::lastVoice).asMilliseconds() > 500){
				injured::lastVoice = lastVoice = time;
				nextVoice = sf::seconds(5.0f + 15.0f * random::random());
				currentVoice = si::playRandomSound(this, id);
			}
		}
	}
	Animatable::tick(time, dt);
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
	for(size_t i = 0; i < applied.size(); i++){
		customJournal->lines.push_back("");
		customJournal->lines.push_back(
			"Treated with " + resourceToString(applied[i].first)
			+ (applied[i].second ? "." : ", but it did not seem to help.")
			);
	}
	setAnimation(state(INJURED_STATES - 1 + progress - journal->requirements.size()));
}

bool Injured::isHealed(){
	return progress >= journal->requirements.size();
}

bool Injured::isDead(){
	return dead;
}

bool Injured::use(ItemStack& is){
	if(is.amount > 0 && !isHealed()){
		for(rbPair pr : applied){
			if(is.item.type == pr.first){
				return false;
			}
		}
		bool snd = false;
		for(Resource r : journal->requirements){
			if(is.item.type == r){
				snd = true;
				break;
			}
		}
		rbPair pair = rbPair(is.item.type, snd);

		applied.push_back(pair);
		if(pair.second){
			progress++;
		}
		updateAnimation();
		is.amount--;
		si::playSound(this, "resources." + resourceToString(is.item.type));
		return true;
	}
	return false;
}