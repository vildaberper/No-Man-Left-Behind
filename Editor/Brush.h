#pragma once

#include <string>
#include <vector>

#include "SFMLI.h"
#include "World.h"
#include "Random.h"
#include "MathHelper.h"

class Brush{
public:
	Brush();
	~Brush();

	bool paint(const float& x, const float& y, const float& radius, const float& density, Manager*& manager, World*& world, const Layer& layer);

	float scaleLower = 1.0f;
	float scaleUpper = 2.0f;
	std::vector<std::string> objects;
};