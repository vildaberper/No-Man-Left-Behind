#pragma once

#include <string>

#include "SFMLI.h"
#include "GameConstants.h"

enum Resource{
	PENICILLIN,
	FORCEPS,
	ALCOHOL,
	MORPHINE,
	SUTURE_KIT,
	SCALPEL,
	GAUZE
};

static const std::string resourceToString(const Resource& resource){
	switch(resource){
	case PENICILLIN:
		return "Penicillin";
		break;
	case FORCEPS:
		return "Forceps";
		break;
	case ALCOHOL:
		return "Alcohol";
		break;
	case MORPHINE:
		return "Morphine";
		break;
	case SUTURE_KIT:
		return "Suture kit";
		break;
	case SCALPEL:
		return "Scalpel";
		break;
	case GAUZE:
		return "Gauze";
		break;
	}
	return "Penicillin";
}

static const Resource parseResource(const std::string& resource){
	if(resource == "Penicillin"){
		return PENICILLIN;
	}
	else if(resource == "Forceps"){
		return FORCEPS;
	}
	else if(resource == "Alcohol"){
		return ALCOHOL;
	}
	else if(resource == "Morphine"){
		return MORPHINE;
	}
	else if(resource == "Suture kit"){
		return SUTURE_KIT;
	}
	else if(resource == "Scalpel"){
		return SCALPEL;
	}
	else if(resource == "Gauze"){
		return GAUZE;
	}
	return PENICILLIN;
}

static const sf::Time getResourceUseTime(const Resource& resource){
	switch(resource){
	case PENICILLIN:
		return sf::seconds(gc::useTimePennicillin);
		break;
	case FORCEPS:
		return sf::seconds(gc::useTimeForceps);
		break;
	case ALCOHOL:
		return sf::seconds(gc::useTimeAlcohol);
		break;
	case MORPHINE:
		return sf::seconds(gc::useTimeMorphine);
		break;
	case SUTURE_KIT:
		return sf::seconds(gc::useTimeSutureKit);
		break;
	case SCALPEL:
		return sf::seconds(gc::useTimeScalpel);
		break;
	case GAUZE:
		return sf::seconds(gc::useTimeGauze);
		break;
	}
	return sf::seconds(1.0f);
}

class Item{
public:
	Item();
	Item(Resource resource);
	~Item();

	Resource type;
};