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

	bool isAlive(){
		return health > 0;
	}
private:
	Vector position;
	Vector velocity;

	float health;
};