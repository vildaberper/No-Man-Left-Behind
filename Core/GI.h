#pragma once

#include "Drawable.h"
#include "SFMLI.h"
#include "Logger.h"

namespace gi{
	static const float TARGET_WIDTH = 1920;
	static const float TARGET_HEIGHT = 1080;

	extern sf::RenderWindow* renderWindow;
	extern unsigned int drawCalls;
	extern unsigned long frameCount;
	extern sf::Time begin;
	extern float cameraX;
	extern float cameraY;

	// Initialize \ Finalize
	bool initalize(sf::RenderWindow*& rw);

	bool finalize();

	// StartOfFrame
	bool startOfFrame();

	float dx();
	float dy();

	// Drawcall
	bool draw(drawable::Drawable* drawable, const sf::Time& time);

	// endOfFrame
	bool endOfFrame();
}