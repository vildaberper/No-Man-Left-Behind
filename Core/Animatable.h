#pragma once

#include "Manager.h"
#include "Drawable.h"

enum AnimatableType{
	DIRECTIONAL,
	STATES
};

class Animatable : public drawable::Drawable{
public:
	Animatable();
	virtual ~Animatable();

	void applyAnimation(Manager* m, const std::string& animation);

	bool hasAnimation(const std::string& animation);

	std::string state(const unsigned int& state);

	virtual void move(const float& dt);

	void setAnimationType(const AnimatableType& animatableType, const unsigned int& numStates = 1);
	AnimatableType getAnimationType();

	void setNumStates(const unsigned int& numStates);
	unsigned int getNumStates();
	void setAnimation(const std::string& animation);
private:
	void applyAnimation(Manager* m, const std::string& category, const std::string& name);
	AnimatableType animatableType;
	unsigned int numStates = 1;
};