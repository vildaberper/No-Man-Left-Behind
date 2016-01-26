#pragma once

#include "SFMLI.h"

static void drawSprite(sf::RenderWindow* rw, sf::Sprite* s){	// TODO: Drawable, not Sprite!
	rw->draw(*s);
}