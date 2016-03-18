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
	if(animation.length() > 5 && animation.substr(0, 5) == "civil"){
		type = CIVIL;
	}
	else if(animation.length() > 7 && animation.substr(0, 7) == "soldier"){
		type = SOLDIER;
	}
	else if(animation.length() > 7 && animation.substr(0, 7) == "general"){
		type = GENERAL;
	}
	setAnimationType(STATES, 5);
	applyAnimation(manager, id = animation);
	Injured::journal = journal;
	Injured::progress = progress;
	customJournal->deathTimer = journal->deathTimer;
	updateAnimation();
	cb.shouldCollide = true;
	cb.renderOffset = 0.9f;
	cb.offset = Vector(0.2f, 0.1f);
	cb.size = Vector(0.6f, 0.8f);
	nextVoice = sf::seconds(random::randomFloat(gc::injuredVoiceUpper, gc::injuredVoiceLower));
	deathBar.customColors = true;
	deathBar.bgColor = sf::Color(185, 185, 185, 155);
	deathBar.oColor = sf::Color(5, 5, 5, 255);
	deathBar.size = Vector(250.0f, 40.0f);
}

void Injured::tick(const sf::Time& time, const float& dt){
	if(isDead() || isHealed()){
		si::stopSound(currentVoice);
	}
	else{
		if(time - lastVoice > nextVoice){
			// Make sure two voices does not start at the same time
			if((time - injured::lastVoice).asMilliseconds() > 500){
				injured::lastVoice = lastVoice = time;
				nextVoice = sf::seconds(random::randomFloat(gc::injuredVoiceUpper, gc::injuredVoiceLower));
				currentVoice = si::playRandomSoundV(this, id, gc::injuredVoiceVolume);
			}
		}
	}

	timer += sf::seconds(dt * (hasSeenJournal ? 1.0f : gc::timerRateNotSeen));
	deathBar.progress = 1.0f - (timer / customJournal->deathTimer);
	if(hasTimer() && timer > customJournal->deathTimer){
		dead = true;
		updateAnimation();
	}

	Animatable::tick(time, dt);
}

void Injured::updateAnimation(){
	deathBar.pbColor = injuredState() == 1 ? sf::Color(10, 10, 10, 255) : sf::Color(104, 24, 24, 255);
	customJournal->lines.clear();
	for(size_t i = 0; i < journal->lines.size(); i++){
		std::string line = journal->lines[i];
		size_t index;

		if((index = line.find("%state%")) != std::string::npos){
			std::string state;
			switch(injuredState()){
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
		if(line.length() > 6 && line.substr(0, 6) == "Name: "){
			name = line.substr(6);
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
	if(isDead()){
		customJournal->lines.push_back("");
		customJournal->lines.push_back("Patient is dead.");
	}
	setAnimation(state(isDead() ? 0 : (INJURED_STATES - 1 + progress - journal->requirements.size())));
}

unsigned char Injured::injuredState(){
	return unsigned char(isDead() ? 0 : (INJURED_STATES - 1 + progress - journal->requirements.size()));
}

bool Injured::isHealed(){
	return !isDead() && progress >= journal->requirements.size();
}

bool Injured::isDead(){
	return dead;
}

bool Injured::hasTimer(){
	return !isDead() && !isHealed() && customJournal->deathTimer != sf::milliseconds(0);
}

bool Injured::survived(){
	if(!hasCheckedSurvival){
		float r = random::randomFloat();

		switch(injuredState()){
		case 0:
			survived_ = r <= gc::survive0Rate;
			break;
		case 1:
			survived_ = r <= gc::survive1Rate;
			break;
		case 2:
			survived_ = r <= gc::survive2Rate;
			break;
		case 3:
			survived_ = r <= gc::survive3Rate;
			break;
		case 4:
			survived_ = r <= gc::survive4Rate;
			break;
		}
		hasCheckedSurvival = true;
	}
	return survived_;
}

bool Injured::use(ItemStack& is){
	if(is.amount > 0 && !isHealed() && !isDead()){
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
			if(injuredState() < 3){
				customJournal->deathTimer *= 2.0f;
				timer = sf::milliseconds(0);
			}
			else{
				customJournal->deathTimer = sf::milliseconds(0);
			}
		}
		updateAnimation();
		is.amount--;
		si::playSound(this, "resources." + resourceToString(is.item.type));
		return true;
	}
	return false;
}