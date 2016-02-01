#pragma once

#include "Menu.h"
#include "SFMLI.h"
#include "Logger.h"
#include "Drawable.h"
#include "Constants.h"

namespace gi{
	static const float TARGET_WIDTH = 1920;
	static const float TARGET_HEIGHT = 1080;

	extern sf::RenderWindow* renderWindow;
	extern unsigned int drawCalls;
	extern unsigned long frameCount;
	extern sf::Time begin;
	extern float cameraX;
	extern float cameraY;
	extern sf::Font menuFont;

	// Initialize \ Finalize
	bool initalize(sf::RenderWindow*& rw);

	bool finalize();

	// StartOfFrame
	bool startOfFrame();

	float dx();
	float dy();

	// Drawcall
	void draw(drawable::Drawable* drawable, const sf::Time& time);

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h);

	void draw(Menu* menu, const sf::Time& time);

	// endOfFrame
	bool endOfFrame();
}