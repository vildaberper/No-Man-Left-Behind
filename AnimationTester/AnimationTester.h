#pragma once

#include "GI.h"
#include "SFMLI.h"
#include "World.h"
#include "Manager.h"
#include "Constants.h"
#include "MathHelper.h"

class AnimationTester : public InputListener{
public:
	AnimationTester();
	~AnimationTester();

	void run();

	void load();

	void save();

	virtual void on(KeyboardEvent& event);
	virtual void on(MouseWheelEvent& event);
private:
	bool repeat = true;

	Manager* manager;

	World* world;

	drawable::Drawable* d = nullptr;
	drawable::Animation* a = nullptr;

	File pngFile;
	File txtFile;
};