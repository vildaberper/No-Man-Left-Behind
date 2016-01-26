#pragma once

#include <vector>

#include "Entity.h"

class World{
public:
	World();
	~World();
private:
	std::vector<Entity*> entities;

	void cleanAll(const bool& all);
};