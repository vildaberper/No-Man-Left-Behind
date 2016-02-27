#include "Truck.h"

Truck::Truck(){

}

Truck::~Truck(){

}

void Truck::initialize(Manager* m){
	setAnimationType(DIRECTIONAL);
	applyAnimation(m, "truck");

	currentAnimation = nextAnimation = "rightidle";
	scale = 1.0f;
	cb.shouldCollide = true;
	cb.offset = Vector(0.09f, 0.7f);
	cb.size = Vector(0.89f, 0.28f);
	cb.renderOffset = 0.8f;
	speed = 500.0f;
	targetspeed = 500.0f;
	acceleration = 100.0f;
}

void Truck::move(const float& dt){
	if(speed > targetspeed){
		speed -= acceleration * dt;
		if(speed < targetspeed){
			speed = targetspeed;
		}
	}
	else if(speed > targetspeed){
		speed += acceleration * dt;
		if(speed > targetspeed){
			speed = targetspeed;
		}
	}

	if(speed > 0.0f){
		playedStopped = false;
		if(playingStopping){
			si::stopSound(soundId);
			playingStopping = false;
		}
		if(!playingRunning){
			playingRunning = true;
			soundId = si::playSound("truck.running", true);
		}
	}
	else{
		if(playingRunning){
			si::stopSound(soundId);
			playingRunning = false;
		}
		if(!playingStopping && !playedStopped){
			playingStopping = true;
			soundId = si::playSound("truck.stopping");
		}
	}

	velocity.x = speed;
	Animatable::move(dt);
}