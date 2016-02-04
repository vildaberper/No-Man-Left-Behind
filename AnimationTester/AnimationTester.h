#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

class AnimationTester{
public:
	AnimationTester();
	~AnimationTester();

	void run();

	void load();

	void save();

	const void keyboardListener(KeyboardEvent& event);
	const void mouseWheelListener(MouseWheelEvent& event);
private:
	sf::RenderWindow* window;

	Manager* manager;

	World* world;

	drawable::Drawable* d = NULL;
	drawable::Animation* a = NULL;

	File pngFile;
	File txtFile;
};