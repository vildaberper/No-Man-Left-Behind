#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

static const float SNAP = 5.0f;

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

	bool dragging = false;
	bool targeting = false;
	Target* target = NULL;
};