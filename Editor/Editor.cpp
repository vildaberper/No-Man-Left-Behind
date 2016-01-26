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
	/*
		File API test
	*/
	File f;
	f = f.parent();
	cout << f.path() << ' ' << round(f.size() / 1024.0f) << "kB" << endl;
	for (File c : f.listFiles()){
		cout << "  " << c.name() << ' ' << round(c.size() / 1024.0f) << "kB" << endl;
	}

	/*
		Configuration API test
	*/
	c::initialize();
	printf("\n");
	printf(c::textureDir.path().data());
	printf("\n");

	/*
		SFML & Manager API test
	*/
	window = new RenderWindow(VideoMode(500, 500), "SFML works!");
	manager = new Manager();
	manager->initialize(window);
	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);
	shape.setOrigin(100, 100);

	float dx = 0.0f;
	float dy = 0.0f;

	keyboardListenerId = manager->inputManager->registerListener(keyboardListenerW);
	mouseButtonListenerId = manager->inputManager->registerListener(mouseButtonListenerW);
	mouseMoveListenerId = manager->inputManager->registerListener(mouseMoveListenerW);
	mouseWheelListenerId = manager->inputManager->registerListener(mouseWheelListenerW);

	while (window->isOpen()){
		manager->tick(window, 0, 0);

		window->clear();

		dx += 0.0001f;
		dy += 0.0001f;
		shape.setPosition(250, 250);
		shape.rotate(0.01f);
		shape.setFillColor(Color(int(255 * abs(sin(dx))), int(255 * abs(cos(dy / 2))), int(255 * abs(cos(dy))), int(55 + 200.0f * abs(cos(dy)))));
		shape.setScale(abs(sin(dx)), abs(cos(dy)));
		window->draw(shape);
		window->display();
	}
	manager->finalize(window);
	delete manager;
	delete window;
}

const void Editor::keyboardListener(KeyboardEvent event){
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