#pragma once

#include "SFMLI.h"
#include "World.h"
#include "Manager.h"

class Editor{
public:
	Editor();
	~Editor();

	void run();

	const void keyboardListener(KeyboardEvent event);
	const void mouseButtonListener(MouseButtonEvent event);
	const void mouseMoveListener(MouseMoveEvent event);
	const void mouseWheelListener(MouseWheelEvent event);
private:
	sf::RenderWindow* window;
	Manager* manager;

	World* world;

	unsigned long keyboardListenerId;
	unsigned long mouseButtonListenerId;
	unsigned long mouseMoveListenerId;
	unsigned long mouseWheelListenerId;
};