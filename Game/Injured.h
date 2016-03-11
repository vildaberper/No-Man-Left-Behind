#pragma once

#include <map>
#include <vector>

#include "Item.h"
#include "Random.h"
#include "Journal.h"
#include "ItemStack.h"
#include "Animatable.h"

static const unsigned char INJURED_STATES = 5;

typedef std::pair<Resource, bool> rbPair;

namespace injured{
	extern sf::Time lastVoice;
}

class Injured: public Animatable{
public:
	Injured();
	~Injured();

	void initialize(Manager* manager, const std::string& animation, Journal* journal);

	virtual void tick(const sf::Time& time, const float& dt);

	void updateAnimation();

	bool isHealed();
	bool isDead();

	bool use(ItemStack& is);

	Journal* journal;
	Journal* customJournal;
	size_t progress;

	sf::Time timer;

	sf::Time lastVoice;
	sf::Time nextVoice;
	unsigned long currentVoice = 0;

	std::string id;

	std::vector<rbPair> applied;

	bool hasSeenJournal = false;

	bool dead = false;
};