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
#include "InputManager.h"

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
	extern sf::Font textFont;

	extern bool showCursor;
	extern bool hasCursor;
	extern drawable::Drawable* cursor;

	extern Vector relativeOffset;

	extern InputManager* inputManager;

	void zoom(const float& zoom);

	void camera(const float& dt);

	void resetCamera();

	// Initialize \ Finalize
	bool initalize();

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

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h, const bool& drawElementBackgrounds);

	void draw(Menu* menu, const sf::Time& time);

	void draw(ProgressBar* progressbar, const sf::Time& time);

	void draw(TexBar* texbar, const float& x, const float& y, const float& w, const float& h, const bool& darken = false);

	void draw(const std::vector<std::string>& text, const float& x, const float& y, const float& w, const float& h, const sf::Font& font = textFont);

	void darken(const float& darkness);

	void draw(CoreSprite& sprite, const float& x, const float& y, const float& w, const float& h, const float& a = 1.0f);

	void background(CoreSprite& sprite);

	void backgroundColor(const sf::Color& color);

	// endOfFrame
	bool endOfFrame();
}