#include "Truck.h"

Truck::Truck(){

}

Truck::~Truck(){

}

void Truck::initialize(Manager* m){
	animatableType = DIRECTIONAL;
	apply(m, "truck");

	currentAnimation = nextAnimation = "rightidle";
	scale = 3.0f;
	cb.shouldCollide = true;
	cb.offset = Vector(0.3f, 0.75f);
	cb.size = Vector(0.4f, 0.2f);
	cb.renderOffset = 0.84f;
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