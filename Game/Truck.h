#pragma once

#include <string>

#include "SI.h"
#include "Manager.h"
#include "Animatable.h"

class Truck : public Animatable{
public:
	Truck();
	~Truck();

	void initialize(Manager* m);

	virtual void move(const float& dt);

	float speed;
	float targetspeed;
	float acceleration;

	bool playingRunning = false;
	bool playingStopping = false;
	bool playedStopped = false;
	unsigned long soundId;
};