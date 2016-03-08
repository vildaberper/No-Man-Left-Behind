#pragma once

#include <map>
#include <vector>

#include "Item.h"
#include "Journal.h"
#include "ItemStack.h"
#include "Animatable.h"

static const unsigned char INJURED_STATES = 5;

class Injured: public Animatable{
public:
	Injured();
	~Injured();

	void initialize(Manager* manager, const std::string& animation, Journal* journal);

	void updateAnimation();

	bool isHealed();

	bool use(ItemStack& is);

	Journal* journal;
	Journal* customJournal;
	size_t progress;

	sf::Time timer;

	std::string id;

	bool voice = false;
	unsigned long currentVoice = 0;
};