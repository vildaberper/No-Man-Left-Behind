#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

class CollisionEditor : public InputListener{
public:
	CollisionEditor();
	~CollisionEditor();

	void run();
	void update();

	void on(KeyboardEvent& event);
	void on(MouseButtonEvent& event);
	void on(MouseMoveEvent& event);
private:
	Manager* manager;

	World* world;

	drawable::Drawable* d = NULL;

	bool dragging = false;

	std::string* current = NULL;
	std::string* selectedString = NULL;
};
