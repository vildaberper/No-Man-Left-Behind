#pragma once

#include "Menu.h"
#include "SFMLI.h"
#include "Logger.h"
#include "TexBar.h"
#include "Drawable.h"
#include "Constants.h"
#include "CoreSprite.h"
#include "MathHelper.h"
#include "ProgressBar.h"

namespace gi{
	static const float TARGET_WIDTH = 1920;
	static const float TARGET_HEIGHT = 1080;

	extern sf::RenderWindow* renderWindow;
	extern float cameraX;
	extern float cameraY;
	extern float cameraZ;

	extern bool smoothCamera;
	extern float cameraSmoothness;
	extern float cameraTargetX;
	extern float cameraTargetY;

	extern bool logAlwaysActive;

	extern float WIDTH;
	extern float HEIGHT;

	extern bool collisionBoxes;

	extern sf::Font menuFont;

	extern bool showCursor;
	extern bool hasCursor;
	extern drawable::Drawable* cursor;

	void zoom(const float& zoom);

	void camera(const float& dt);

	void resetCamera();

	// Initialize \ Finalize
	bool initalize(sf::RenderWindow*& rw);

	bool finalize();

	// StartOfFrame
	bool startOfFrame();

	float wx();
	float wy();

	float dx();
	float dy();

	float dxiz();
	float dyiz();

	float sx(float x);
	float sy(float y);

	float wx(float x);
	float wy(float y);

	// Drawcall
	void draw(sf::Sprite& sprite);
	void draw(CoreSprite& sprite);

	void draw(drawable::Drawable* drawable, const sf::Time& time);

	void draw(const logger::LogEntry& logEntry, const float& x, const float& y, const float& w, const float& h);
	void drawLog();

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h);

	void draw(Menu* menu, const sf::Time& time);

	void draw(ProgressBar* progressbar, const sf::Time& time);

	void draw(TexBar* texbar, const float& x, const float& y, const float& w, const float& h);

	void darken(const float& darkness);

	void background(CoreSprite& sprite);

	// endOfFrame
	bool endOfFrame();
}