#pragma once

#include "SFMLI.h"

class CoreSprite{
public:
	CoreSprite(sf::Sprite* sprite, int w, int h);
	~CoreSprite();

	sf::Sprite* sprite();

	int w();
	int h();
private:
	sf::Sprite* sprite_;

	int w_;
	int h_;
};