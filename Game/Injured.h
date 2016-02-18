#pragma once

#include <vector>
#include <map>

#include "Animatable.h"
#include "Item.h"

enum InjuredState {
	DEAD,
	CRITICAL,
	SERIOUS,
	FAIR,
	HEALED
};

static const InjuredState nextInjure(const InjuredState& current) {
	switch(current) {
	case DEAD:
		return CRITICAL;
		break;
	case CRITICAL:
		return SERIOUS;
		break;
	case SERIOUS:
		return FAIR;
		break;
	case FAIR:
		return HEALED;
		break;
	case HEALED:
		return HEALED;
		break;
	}
	return DEAD;
}

static const InjuredState previusInjuredState(const InjuredState& current) {
	switch(current) {
	case DEAD:
		return DEAD;
		break;
	case CRITICAL:
		return DEAD;
		break;
	case SERIOUS:
		return CRITICAL;
		break;
	case FAIR:
		return SERIOUS;
		break;
	case HEALED:
		return FAIR;
		break;
	}
	return DEAD;
}

static const InjuredState parseInjuredState(const std::string& s) {
	if(s == "DEAD") {
		return DEAD;
	}
	else if(s == "CRITICAL") {
		return CRITICAL;
	}
	else if(s == "SERIOUS") {
		return SERIOUS;
	}
	else if(s == "FAIR") {
		return FAIR;
	}
	else if(s == "HEALED") {
		return HEALED;
	}
	return DEAD;
}

static const std::string injuredStateToString(const InjuredState& Injure) {
	if(Injure == DEAD) {
		return "DEAD";
	}
	else if(Injure == CRITICAL) {
		return "CRITICAL";
	}
	else if(Injure == SERIOUS) {
		return "SERIOUS";
	}
	else if(Injure == FAIR) {
		return "FAIR";
	}
	else if(Injure == HEALED) {
		return "HEALED";
	}
	return "DEAD";
}

enum Injure{
	OPEN_WOUND
};

/*
	toString/parse Injure
*/

namespace injured{
	static std::map<Injure, std::vector<Resource>> createRequirements(){
		std::map<Injure, std::vector<Resource>> r;

		r[OPEN_WOUND] = std::vector<Resource>();
		r[OPEN_WOUND].push_back(GAUZE);
		r[OPEN_WOUND].push_back(PENICILLIN);

		return r;
	}
}

static const std::map<Injure, std::vector<Resource>> requirements = injured::createRequirements();

class Injured: public Animatable {
public:
	Injured();
	~Injured();

	Injure injure;
	size_t progress;

	sf::Time timer;
	InjuredState injuredState; // Same as progress?
};