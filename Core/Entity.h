#pragma once

#include "Logger.h"
#include "Vector.h"
#include "SFMLI.h"

class Entity{
public:
	Entity(){
		position = Vector(0, 0);
		velocity = Vector(0,0);
		health = 1.0f;
	};
	virtual ~Entity(){

	};

	virtual void move(const float& dt){
		position += velocity * dt;
	}

	virtual void tick(const sf::Time& time, const float& dt){
		
	};

	bool isAlive(){
		return health > 0.0f;
	}

	void kill(){
		health = -1.0f;
	}
	Vector velocity;
	Vector position;
	Vector size;
	float health;
};