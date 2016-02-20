#include "Editor.h"

#include <string>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;
using namespace math;

Editor* editor;

void keyboardListenerW(KeyboardEvent& event){
	editor->keyboardListener(event);
}
void mouseButtonListenerW(MouseButtonEvent& event){
	editor->mouseButtonListener(event);
}
void mouseMoveListenerW(MouseMoveEvent& event){
	editor->mouseMoveListener(event);
}
void mouseWheelListenerW(MouseWheelEvent& event){
	editor->mouseWheelListener(event);
}

Editor::Editor(){
	editor = this;
	selectedString = new std::string();
	selectedBackground = new std::string();
}

Editor::~Editor(){
	delete selectedString;
	delete selectedBackground;
}

void Editor::run(){
	sf::Clock fg;
	c::initialize();
	logger::timing("Constants initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	fg.restart();
	gi::initalize(window);
	logger::timing("Graphics interface initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	manager = new Manager();
	manager->initialize(window);
	logger::timing("Manager initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	world = new World(manager);
	file = File().child("world.txt");
	world->load(file);
	world->background = manager->spriteManager->getBackground(world->backgroundName);

	vector<string> cs = manager->spriteManager->categories();
	Menu* cm = new Menu();
	cm->position.x = 0;
	cm->position.y = 0;
	cm->size.x = gi::TARGET_WIDTH;
	cm->size.y = 50;
	cm->type = HORIZONTAL;
	cm->hidden = false;
	for (size_t c = 0; c < cs.size(); c++){
		Menu* tm = new Menu();
		MenuItem* mi = new MenuItem();
		mi->title = cs[c];
		mi->toggle = tm;
		cm->items.push_back(mi);
		float w = cm->size.x / cs.size();
		tm->position.x = w * c;
		tm->position.y = 50;
		tm->size.x = w;
		tm->size.y = gi::TARGET_HEIGHT - 50 - 70;
		tm->type = VERTICAL;
		tm->hidden = true;
		vector<string> ms = manager->spriteManager->members(mi->title);
		for (string t : ms){
			MenuItem* ti = new MenuItem();
			ti->title = t;
			ti->type = TEXTURE;
			ti->sprite = manager->spriteManager->getSprite(mi->title, t);
			ti->closeOnClick = true;
			ti->selectedPrefix = cs[c] + '.';
			ti->selectedString = selectedString;
			tm->items.push_back(ti);
		}
		manager->menuManager->menus["categories." + mi->title] = tm;
	}
	manager->menuManager->menus["categories"] = cm;

	keyboardListenerId = manager->inputManager->registerListener(keyboardListenerW);
	mouseButtonListenerId = manager->inputManager->registerListener(mouseButtonListenerW);
	mouseMoveListenerId = manager->inputManager->registerListener(mouseMoveListenerW);
	mouseWheelListenerId = manager->inputManager->registerListener(mouseWheelListenerW);

	Menu* layerM = new Menu();
	layerM->hidden = false;
	layerM->position = Vector(10, gi::TARGET_HEIGHT - 50 - 10);
	layerM->size = Vector(200, 50);
	layerM->type = HORIZONTAL;
	layerMenu = new MenuItem();
	layerMenu->closeOnClick = false;
	layerMenu->type = TEXT;
	layerMenu->title = "LAYER0";
	layerM->items.push_back(layerMenu);
	manager->menuManager->menus["layer"] = layerM;

	Menu* spriteM = new Menu();
	spriteM->hidden = false;
	spriteM->position = Vector(gi::TARGET_WIDTH / 2 - 300, gi::TARGET_HEIGHT - 50 - 10);
	spriteM->size = Vector(600, 50);
	spriteM->type = HORIZONTAL;
	spriteMenu = new MenuItem();
	spriteMenu->closeOnClick = false;
	spriteMenu->type = TEXTURE;
	spriteMenu->title = "NONE";
	spriteM->items.push_back(spriteMenu);
	manager->menuManager->menus["sprite"] = spriteM;

	cs = manager->spriteManager->backgrounds();
	Menu* backgroundC = new Menu();
	backgroundC->hidden = true;
	backgroundC->type = VERTICAL;
	backgroundC->position = Vector(gi::TARGET_WIDTH - 310, gi::TARGET_HEIGHT - 50 - 10 - 400);
	backgroundC->size = Vector(300, 400);
	for (string b : cs){
		MenuItem* ti = new MenuItem();
		ti->title = b;
		ti->type = TEXTURE;
		ti->sprite = new Sprite(*manager->spriteManager->getBackground(b));
		ti->closeOnClick = true;
		ti->selectedString = selectedBackground;
		backgroundC->items.push_back(ti);
	}
	manager->menuManager->menus["backgroundC"] = backgroundC;

	Menu* backgroundM = new Menu();
	backgroundM->hidden = false;
	backgroundM->position = Vector(gi::TARGET_WIDTH - 310, gi::TARGET_HEIGHT - 50 - 10);
	backgroundM->size = Vector(300, 50);
	backgroundM->type = HORIZONTAL;
	backgroundMenu = new MenuItem();
	backgroundMenu->toggle = backgroundC;
	backgroundMenu->closeOnClick = false;
	backgroundMenu->type = TEXTURE;
	if (world->backgroundName.length() > 0){
		backgroundMenu->title = world->backgroundName;
		if (world->background != NULL){
			backgroundMenu->sprite = new Sprite(*world->background);
		}
	}
	else{
		backgroundMenu->title = "NONE";
	}
	backgroundM->items.push_back(backgroundMenu);
	manager->menuManager->menus["background"] = backgroundM;

	window->setFramerateLimit(60);
	while (gi::startOfFrame()){
		if (manager->inputManager->isPressed(sf::Keyboard::Key::Z)){
			gi::zoom(gi::cameraZ + 1.0f * gi::cameraZ * world->dt());
		}
		if (manager->inputManager->isPressed(sf::Keyboard::Key::X)){
			gi::zoom(gi::cameraZ - 1.0f * gi::cameraZ * world->dt());
		}

		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		world->render();

		manager->menuManager->draw(world->time());

		gi::endOfFrame();
	}
	world->save(file);
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
}

const void Editor::keyboardListener(KeyboardEvent& event){
	if (event.isCancelled()){
		return;
	}
	if (event.pressed()){
		switch (event.key()){
		case Keyboard::Escape:
			world->save(file);
			window->close();
			break;
		case Keyboard::Delete:
			if (target != NULL){
				target->drawable->kill();
				delete target;
				target = NULL;
				targeting = false;
			}
			break;
		case Keyboard::S:
			if (manager->inputManager->isPressed(Keyboard::LControl)){
				world->save(file);
			}
			break;
		case Keyboard::Add:
			if (target != NULL){
				target->drawable->scale *= 1.2f;
			}
			break;
		case Keyboard::Subtract:
			if (target != NULL){
				target->drawable->scale *= (5.0f / 6.0f);
			}
			break;
		case Keyboard::C:
			if(event.first()){
				gi::collisionBoxes = !gi::collisionBoxes;
			}
			break;
		}
	}
}

const void Editor::mouseButtonListener(MouseButtonEvent& event){
	if (event.isCancelled()){
		if (event.button() == Mouse::Button::Left && event.pressed()){
			if (selectedString->length() > 0){
				spriteMenu->title = *selectedString;
				spriteMenu->sprite = manager->spriteManager->getSprite(*selectedString);
			}
			if (selectedBackground->length() > 0){
				backgroundMenu->title = *selectedBackground;
				delete backgroundMenu->sprite;
				backgroundMenu->sprite = new Sprite(*manager->spriteManager->getBackground(*selectedBackground));
				world->backgroundName = *selectedBackground;
				world->background = manager->spriteManager->getBackground(*selectedBackground);
			}
		}
		return;
	}
	switch (event.button()){
	case Mouse::Button::Right:
		dragging = event.pressed();
		break;
	case Mouse::Button::Middle:
		if(event.pressed() && target != NULL){
			*selectedString = target->drawable->reference;
			if(selectedString->length() > 0){
				spriteMenu->title = *selectedString;
				spriteMenu->sprite = manager->spriteManager->getSprite(*selectedString);
			}
		}
		break;
	case Mouse::Button::Left:
		targeting = target != NULL && event.pressed();

		if (!targeting && event.pressed()){
			if (selectedString != NULL && selectedString->length() > 0){
				Sprite* s = manager->spriteManager->getSprite(*selectedString);
				drawable::Drawable* d = new drawable::Drawable();
				drawable::Animation* a = new drawable::Animation();
				d->position = Vector(
					gi::cameraX - gi::TARGET_WIDTH / 2 + event.x() / gi::dx() - s->getGlobalBounds().width / 2,
					gi::cameraY - gi::TARGET_HEIGHT / 2 + event.y() / gi::dy() - s->getGlobalBounds().height / 2
					);
				d->scale = 1.0f;
				a->textures.push_back(*selectedString);
				a->sprites.push_back(s);
				a->timing = milliseconds(1000);
				d->animations["default"] = a;
				d->currentAnimation = d->nextAnimation = "default";
				world->addDrawable(d, selectedLayer);
				if (target != NULL){
					delete target;
				}
				targeting = true;
				d->highlight = true;
				target = new Target(d, selectedLayer, s->getLocalBounds().width * gi::dx() / 2, s->getLocalBounds().height * gi::dy() / 2);
				mouseMoveListener(MouseMoveEvent(event.x(), event.y(), 0, 0));
			}
		}
		break;
	}
}

const void Editor::mouseMoveListener(MouseMoveEvent& event){
	if (event.isCancelled()){
		return;
	}
	if (dragging){
		gi::cameraX -= event.dx() / gi::dx();
		gi::cameraY -= event.dy() / gi::dy();
	}
	if (!targeting){
		Target* nt;
		if ((nt = world->drawableAt(float(event.x()), float(event.y()), selectedLayer)) != NULL){
			if (target != NULL){
				target->drawable->highlight = false;
				delete target;
			}
			target = nt;
			target->drawable->highlight = true;
		}
		else{
			if (target != NULL){
				target->drawable->highlight = false;
				delete target;
			}
			target = NULL;
		}
	}
	else{
		if (!dragging){
			target->drawable->position.x = gi::cameraX - gi::TARGET_WIDTH / 2 + event.x() / gi::dx() - target->dx / gi::dx();
			target->drawable->position.y = gi::cameraY - gi::TARGET_HEIGHT / 2 + event.y() / gi::dy() - target->dy / gi::dy();

			if (target->layer == LAYER0){
				if (world->background != NULL){
					int w = world->background->getSize().x;
					int h = world->background->getSize().y;
					float x = target->drawable->position.x - w / 2;
					float y = target->drawable->position.y - h / 2;
					x = ceil(x / w) * w;
					y = ceil(y / h) * h;
					target->drawable->position.x = x;
					target->drawable->position.y = y;
				}
			}
			else if (manager->inputManager->isPressed(sf::Keyboard::Key::S)){
				FloatRect tr = target->drawable->getSprite(world->time())->getGlobalBounds();
				for (drawable::Drawable* d : world->drawables[target->layer]){
					FloatRect dr = d->getSprite(world->time())->getGlobalBounds();
					if (interv(dr.left + dr.width, tr.left) < SNAP){
						if (interv(dr.top + dr.height, tr.top) < SNAP){
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
			world->orderDrawables(target->layer);
		}
	}
}

const void Editor::mouseWheelListener(MouseWheelEvent& event){
	if (event.isCancelled()){
		return;
	}
	if (event.delta() > 0){
		selectedLayer = nextLayer(selectedLayer);
	}
	else{
		selectedLayer = previousLayer(selectedLayer);
	}
	if (target != NULL && targeting){
		drawable::Drawable* d = target->drawable;

		world->drawables[target->layer].erase(remove(world->drawables[target->layer].begin(), world->drawables[target->layer].end(), d));
		world->addDrawable(d, selectedLayer);
		target->layer = selectedLayer;
	}
	else{
		targeting = false;
		if (target != NULL){
			target->drawable->highlight = false;
			delete target;
			target = NULL;
		}
	}
	layerMenu->title = layerToString(selectedLayer);
	mouseMoveListener(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
}