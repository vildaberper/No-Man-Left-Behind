#include "Editor.h"

#include <string>
#include <iostream>

#include "Configuration.h"

using namespace sf;
using namespace std;

Editor* editor;

void keyboardListenerW(KeyboardEvent event){
	editor->keyboardListener(event);
}
void mouseButtonListenerW(MouseButtonEvent event){
	editor->mouseButtonListener(event);
}
void mouseMoveListenerW(MouseMoveEvent event){
	editor->mouseMoveListener(event);
}
void mouseWheelListenerW(MouseWheelEvent event){
	editor->mouseWheelListener(event);
}

Editor::Editor(){
	editor = this;
}

Editor::~Editor(){

}

void Editor::run(){
	c::initialize();
	gi::initalize(window);
	manager = new Manager();
	manager->initialize(window);
	world = new World();

	CircleShape shape(100.f);
	shape.setOrigin(100, 100);

	float dx = 0.0f;
	float dy = 0.0f;

	keyboardListenerId = manager->inputManager->registerListener(keyboardListenerW);
	mouseButtonListenerId = manager->inputManager->registerListener(mouseButtonListenerW);
	mouseMoveListenerId = manager->inputManager->registerListener(mouseMoveListenerW);
	mouseWheelListenerId = manager->inputManager->registerListener(mouseWheelListenerW);

	while (gi::startOfFrame()){
		manager->tick(window, world->time(), world->dt());

		window->clear();

		dx += 0.0001f;
		dy += 0.0001f;

		shape.setPosition(640, 360);
		shape.rotate(0.01f);
		shape.setFillColor(Color(int(255 * abs(sin(dx))), int(255 * abs(cos(dy / 2))), int(255 * abs(cos(dy))), int(55 + 200.0f * abs(cos(dy)))));
		shape.setScale(abs(sin(dx)), abs(cos(dy)));
		window->draw(shape);

		gi::endOfFrame();
	}
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
}

const void Editor::keyboardListener(KeyboardEvent event){
	if (event.key() == Keyboard::Escape)
		window->close();
	cout << (event.pressed() ? "press " : "release ") << event.key() << " " << event.first() << endl;
}

const void Editor::mouseButtonListener(MouseButtonEvent event){
	cout << (event.pressed() ? "press " : "release ") << event.button() << " " << event.doubleClick() << " " << event.x() << " " << event.y() << endl;
}

const void Editor::mouseMoveListener(MouseMoveEvent event){
	cout << event.x() << " " << event.y() << endl;
}

const void Editor::mouseWheelListener(MouseWheelEvent event){
	cout << event.delta() << endl;
}