#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

static const float SNAP = 20.0f;

class Editor : public InputListener{
public:
	Editor();
	~Editor();

	void run();

	virtual void on(KeyboardEvent& event);
	virtual void on(MouseButtonEvent& event);
	virtual void on(MouseMoveEvent& event);
	virtual void on(MouseWheelEvent& event);
private:
	sf::RenderWindow* window;

	Manager* manager;

	World* world;

	File file;

	unsigned long inputListenerId;

	bool dragging = false;
	bool targeting = false;
	Target* target = NULL;

	std::string* selectedBackground = NULL;

	std::string* selectedString = NULL;
	Layer selectedLayer = LAYER0;
	MenuItem* layerMenu;

	MenuItem* spriteMenu;
	MenuItem* backgroundMenu;
};