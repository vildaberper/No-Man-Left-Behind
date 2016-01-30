#include "Editor.h"

#include <string>
#include <vector>
#include <iostream>

#include "Configuration.h"
#include "World.h"

using namespace sf;
using namespace std;
using namespace math;

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

	drawable::Drawable* d = new drawable::Drawable();
	drawable::Animation a = drawable::Animation();
	d->position = Vector(500,500);
	d->scale = 1.0f;
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_downleft"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_left"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_upleft"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_up"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_upright"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_right"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_downright"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_down"));
	a.timing = milliseconds(170);
	d->animations["testa"] = a;
	d->currentAnimation = d->nextAnimation = "testa";
	world->addDrawable(d, LAYER0);

	drawable::Drawable* d1 = new drawable::Drawable();
	drawable::Animation a1 = drawable::Animation();
	d1->position = Vector(520, 520);
	d1->scale = 1.0f;
	a1.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_downleft"));
	a1.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_left"));
	a1.timing = milliseconds(170);
	d1->animations["testa"] = a1;
	d1->currentAnimation = d1->nextAnimation = "testa";
	world->addDrawable(d1, LAYER1);

	drawable::Drawable* d2 = new drawable::Drawable();
	drawable::Animation a2 = drawable::Animation();
	d2->position = Vector(500, 500);
	d2->scale = 0.7f;
	a2.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_left"));
	a2.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_downleft"));
	a.sprites.push_back(manager->spriteManager->getSprite("Spritesheet_Floor.Dirt_snow_downright"));
	a2.timing = milliseconds(60);
	d2->animations["testa"] = a2;
	d2->currentAnimation = d2->nextAnimation = "testa";
	world->addDrawable(d2, LAYER1);

	float dx = 0.0f;
	float dy = 0.0f;

	keyboardListenerId = manager->inputManager->registerListener(keyboardListenerW);
	mouseButtonListenerId = manager->inputManager->registerListener(mouseButtonListenerW);
	mouseMoveListenerId = manager->inputManager->registerListener(mouseMoveListenerW);
	mouseWheelListenerId = manager->inputManager->registerListener(mouseWheelListenerW);

	window->setFramerateLimit(60);
	while (gi::startOfFrame()){
		d->rotation += 60 * world->dt();
		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		dx += 2.0f * world->dt();
		dy += 2.1f * world->dt();

		shape.setPosition(640, 360);
		shape.rotate(0.01f);
		shape.setFillColor(Color(int(255 * abs(sin(dx))), int(255 * abs(cos(dy / 2))), int(255 * abs(cos(dy))), int(55 + 200.0f * abs(cos(dy)))));
		shape.setScale(abs(sin(dx)), abs(cos(dy)));
		window->draw(shape);

		world->render();

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
	//cout << (event.pressed() ? "press " : "release ") << event.key() << " " << event.first() << endl;
}

const void Editor::mouseButtonListener(MouseButtonEvent event){
	switch (event.button()){
	case Mouse::Button::Right:
		dragging = event.pressed();
		break;
	case Mouse::Button::Left:
		targeting = target != NULL && event.pressed();
		break;
	}
	//cout << (event.pressed() ? "press " : "release ") << event.button() << " " << event.doubleClick() << " " << event.x() << " " << event.y() << endl;
}

const void Editor::mouseMoveListener(MouseMoveEvent event){
	if (dragging){
		gi::cameraX -= event.dx() / gi::dx();
		gi::cameraY -= event.dy() / gi::dy();
	}
	if (!targeting){
		Target* nt;
		if ((nt = world->drawableAt(float(event.x()), float(event.y()))) != NULL){
			if (target != NULL){
				target->drawable->highlight = false;
			}
			target = nt;
			target->drawable->highlight = true;
		}
		else{
			if (target != NULL){
				target->drawable->highlight = false;
			}
			target = NULL;
		}
	}
	else{
		if (!dragging){
			target->drawable->position.x += event.dx() / gi::dx();
			target->drawable->position.y += event.dy() / gi::dy();

			FloatRect tr = target->drawable->getSprite(world->time())->getGlobalBounds();
			for (drawable::Drawable* d : world->drawables[target->layer]){
				FloatRect dr = d->getSprite(world->time())->getGlobalBounds();

				if (interv(dr.left + dr.width, tr.left) < SNAP){
					if(interv(dr.top + dr.height, tr.top) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
						break;
					}
					else if (interv(dr.top, tr.top) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy()) / gi::dy();
						break;
					}
					else if (interv(dr.top, tr.top + tr.height) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
						break;
					}
					else if (interv(dr.top + dr.height, tr.top + tr.height) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height + dr.height) / gi::dy();
						break;
					}
				}
				else if (interv(dr.left, tr.left + tr.width) < SNAP){
					if (interv(dr.top + dr.height, tr.top) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
						break;
					}
					else if (interv(dr.top, tr.top) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy()) / gi::dy();
						break;
					}
					else if (interv(dr.top, tr.top + tr.height) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
						break;
					}
					else if (interv(dr.top + dr.height, tr.top + tr.height) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height + dr.height) / gi::dy();
						break;
					}
				}
				else if (interv(dr.top + dr.height, tr.top) < SNAP){
					if (interv(dr.left, tr.left) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx()) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
						break;
					}
					else if (interv(dr.left + dr.width, tr.left + tr.width) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
						break;
					}
				}
				else if (interv(dr.top, tr.top + tr.height) < SNAP){
					if (interv(dr.left, tr.left) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx()) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
						break;
					}
					else if (interv(dr.left + dr.width, tr.left + tr.width) < SNAP){
						target->drawable->position.x = (d->position.x * gi::dx() + dr.width - tr.width) / gi::dx();
						target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
						break;
					}
				}
			}
		}
	}
	//cout << event.x() << " " << event.y() << endl;
}

const void Editor::mouseWheelListener(MouseWheelEvent event){
	//cout << event.delta() << endl;
}