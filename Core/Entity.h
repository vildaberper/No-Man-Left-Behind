#pragma once

#include "Vector.h"
#include "SFMLI.h"

class Entity{
public:
	Entity(){};
	~Entity(){};

	void move(const float& dt){
		position += velocity * dt;
	}

	virtual void tick(const sf::Time& time, const float& dt){
		move(dt);
	};

	const bool isAlive(){
		return health > 0;
	}

	void kill(){
		health = 0;
	}
	Vector position;
private:
	Vector velocity;

	float health = 1.0f;
};