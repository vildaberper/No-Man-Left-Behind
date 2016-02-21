#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

class CollisionEditor{
public:
	CollisionEditor();
	~CollisionEditor();

	void run();
	void update();
	const void keyboardListener(KeyboardEvent& event);
	const void mouseButtonListener(MouseButtonEvent& event);
	const void mouseMoveListener(MouseMoveEvent& event);
private:
	sf::RenderWindow* window;

	Manager* manager;

	World* world;

	drawable::Drawable* d = NULL;

	bool dragging = false;

	unsigned long keyboardListenerId;
	unsigned long mouseButtonListenerId;
	unsigned long mouseMoveListenerId;

	std::string* current = NULL;
	std::string* selectedString = NULL;
};
