#include "Editor.h"

#include <string>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;
using namespace math;

Editor::Editor(){
	selectedString = new std::string();
	selectedBackground = new std::string();
}

Editor::~Editor(){
	delete selectedString;
	delete selectedBackground;
}

void Editor::run(){
	c::initialize();
	gi::initalize(window);
	manager = new Manager();
	manager->initialize(window);
	world = new World(manager);
	file = c::worldDir.child("world.txt");
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
	for(size_t c = 0; c < cs.size(); c++){
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
		for(string t : ms){
			MenuItem* ti = new MenuItem();
			ti->title = t;
			ti->type = TEXTURE;
			ti->sprite = manager->spriteManager->getSprite(mi->title, t)->sprite();
			ti->closeOnClick = true;
			ti->selectedPrefix = cs[c] + '.';
			ti->selectedString = selectedString;
			tm->items.push_back(ti);
		}
		manager->menuManager->menus["categories." + mi->title] = tm;
	}
	manager->menuManager->menus["categories"] = cm;

	inputListenerId = manager->inputManager->registerListener(this);

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

	Menu* swapM = new Menu();
	swapM->hidden = false;
	swapM->position = Vector(220, gi::TARGET_HEIGHT - 50 - 10);
	swapM->size = Vector(430, 50);
	swapM->type = HORIZONTAL;
	swapMenu = new MenuItem();
	swapMenu->closeOnClick = false;
	swapMenu->type = TEXT;
	swapMenu->title = "Idle";
	swapM->items.push_back(swapMenu);
	manager->menuManager->menus["swap"] = swapM;

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
	for(string b : cs){
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
	if(world->backgroundName.length() > 0){
		backgroundMenu->title = world->backgroundName;
		if(world->background != NULL){
			backgroundMenu->sprite = new Sprite(*world->background);
		}
	}
	else{
		backgroundMenu->title = "NONE";
	}
	backgroundM->items.push_back(backgroundMenu);
	manager->menuManager->menus["background"] = backgroundM;

	gi::smoothCamera = true;
	gi::cameraSmoothness = 25.0f;

	for(int i = 1; i < 6; i++){
		brushObjects.push_back("Big Trees.Bigtree " + std::to_string(i));
		brushObjects.push_back("Big Trees.Bigtree " + std::to_string(i));
		brushObjects.push_back("Big Trees.Bigtree " + std::to_string(i));
	}
	for(int i = 1; i < 6; i++){
		brushObjects.push_back("Small Props.Stone " + std::to_string(i));
	}
	for(int i = 1; i < 6; i++){
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
		brushObjects.push_back("Big Props.Pine " + std::to_string(i));
	}
	random::initialize();

	while(gi::startOfFrame()){
		if(swapClock.getElapsedTime().asSeconds() > 1.0f){
			if(world->isOrdering()){
				swapMenu->title = layerToString(world->swapLayer) + " - " + to_string(world->swapPosition[world->swapLayer]) + "/" + to_string(world->drawables[world->swapLayer].size());
			}
			else{
				swapMenu->title = "Idle";
			}
			swapClock.restart();
		}

		if(manager->inputManager->isPressed(sf::Keyboard::Key::Z)){
			gi::zoom(gi::cameraZ + gi::cameraZ * world->dt());
			if(target != NULL){
				target->dx += target->dx * world->dt();
				target->dy += target->dy * world->dt();
			}
			on(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
		}
		if(manager->inputManager->isPressed(sf::Keyboard::Key::X)){
			gi::zoom(gi::cameraZ - gi::cameraZ * world->dt());
			if(target != NULL){
				target->dx -= target->dx * world->dt();
				target->dy -= target->dy * world->dt();
			}
			on(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
		}
		if(manager->inputManager->isPressed(sf::Keyboard::Key::B)){
			for(int i = 0; i < max(1, int(brushRadius / 1000)); i++){
				float angle = 2.0f * PI * random::random();
				float radius = brushRadius * random::random();
				float x = cos(angle) * radius;
				float y = sin(angle) * radius;

				x += gi::cameraX - gi::WIDTH / 2 + manager->inputManager->mouseX() / gi::dx();
				y += gi::cameraY - gi::HEIGHT / 2 + manager->inputManager->mouseY() / gi::dy();

				std::string sprite = brushObjects[random::random(brushObjects.size() - 1)];
				CoreSprite* s = manager->spriteManager->getSprite(sprite);
				drawable::Drawable* d = new drawable::Drawable();
				drawable::Animation* a = new drawable::Animation();
				d->scale = brushScaleLower + (brushScaleUpper - brushScaleLower) * random::random();
				a->textures.push_back(sprite);
				a->sprites.push_back(s);
				a->timing = milliseconds(0);
				d->animations["default"] = a;
				d->currentAnimation = d->nextAnimation = "default";
				FloatRect frb = d->getSprite(world->time())->sprite()->getLocalBounds();
				d->position.x = x - (d->cb.offset.x * frb.width + d->cb.size.x / 2 * frb.width);
				d->position.y = y - (d->cb.offset.y * frb.height + d->cb.size.y / 2 * frb.height);
				FloatRect fr = d->bounds(world->time());
				float cx = fr.left + fr.width / 2.0f;
				float cy = fr.top + fr.height / 2.0f;
				bool passed = true;
				d->calcRenderOffset();
				size_t min = world->binarySearchRenderOffset(d->renderOffset() - MAX_COLLISION_DISTANCE, selectedLayer);
				size_t max = world->binarySearchRenderOffset(d->renderOffset() + MAX_COLLISION_DISTANCE, selectedLayer);
				for(size_t i = min; i <= max; i++){
					drawable::Drawable* d0 = world->drawables[selectedLayer][i];
					if(interv(cx, d0->position.x) + interv(cy, d0->position.y) > MAX_COLLISION_DISTANCE){
						continue;
					}
					FloatRect fr0 = d0->bounds(world->time());

					if(fr.intersects(fr0) || math::distance(cx, cy, fr0.left + fr0.width / 2.0f, fr0.top + fr0.height / 2.0f) < brushDensity){
						passed = false;
						break;
					}
				}
				if(passed){
					world->addDrawable(d, selectedLayer);
				}
				else{
					delete d;
					delete a;
				}
			}
		}

		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		gi::camera(world->dt());

		world->render();

		gi::drawLog();

		manager->menuManager->draw(world->time());

		gi::endOfFrame();
	}
	world->save(file);
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
}

void Editor::on(KeyboardEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(event.pressed()){
		switch(event.key()){
		case Keyboard::Escape:
			window->close();
			break;
		case Keyboard::Delete:
			if(target != NULL){
				target->drawable->kill();
				delete target;
				target = NULL;
				targeting = false;
			}
			break;
		case Keyboard::S:
			if(event.first()){
				if(manager->inputManager->isPressed(Keyboard::LControl)){
					world->save(file);
				}
			}
			break;
		case Keyboard::Add:
			if(target != NULL){
				target->drawable->scale *= 1.2f;
			}
			break;
		case Keyboard::Subtract:
			if(target != NULL){
				target->drawable->scale *= (5.0f / 6.0f);
			}
			break;
		case Keyboard::C:
			if(event.first()){
				gi::collisionBoxes = !gi::collisionBoxes;
			}
			break;
		case Keyboard::L:
			if(event.first()){
				gi::logAlwaysActive = !gi::logAlwaysActive;
			}
			break;
		}
	}
}
void Editor::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		if(event.button() == Mouse::Button::Left && event.pressed()){
			if(selectedString->length() > 0){
				spriteMenu->title = *selectedString;
				spriteMenu->sprite = manager->spriteManager->getSprite(*selectedString)->sprite();
			}
			if(selectedBackground->length() > 0){
				backgroundMenu->title = *selectedBackground;
				delete backgroundMenu->sprite;
				backgroundMenu->sprite = new Sprite(*manager->spriteManager->getBackground(*selectedBackground));
				world->backgroundName = *selectedBackground;
				world->background = manager->spriteManager->getBackground(*selectedBackground);
			}
		}
		return;
	}
	switch(event.button()){
	case Mouse::Button::Right:
		dragging = event.pressed();
		break;
	case Mouse::Button::Middle:
		if(event.pressed() && target != NULL){
			*selectedString = target->drawable->reference;
			if(selectedString->length() > 0){
				spriteMenu->title = *selectedString;
				spriteMenu->sprite = manager->spriteManager->getSprite(*selectedString)->sprite();
			}
		}
		break;
	case Mouse::Button::Left:
		targeting = target != NULL && event.pressed();

		if(!targeting && event.pressed()){
			if(selectedString != NULL && selectedString->length() > 0){
				CoreSprite* s = manager->spriteManager->getSprite(*selectedString);
				drawable::Drawable* d = new drawable::Drawable();
				drawable::Animation* a = new drawable::Animation();
				a->textures.push_back(*selectedString);
				a->sprites.push_back(s);
				a->timing = milliseconds(0);
				d->animations["default"] = a;
				d->currentAnimation = d->nextAnimation = "default";
				if(target != NULL){
					delete target;
				}
				targeting = true;
				d->highlight = true;
				target = new Target(d, selectedLayer, s->sprite()->getGlobalBounds().width / 2 * gi::dx(), s->sprite()->getGlobalBounds().height / 2 * gi::dy());
				d->position.x = gi::cameraX - gi::WIDTH / 2 + (event.x() - target->dx) / gi::dx();
				d->position.y = gi::cameraY - gi::HEIGHT / 2 + (event.y() - target->dy) / gi::dy();
				world->addDrawable(d, selectedLayer);
				on(MouseMoveEvent(event.x(), event.y(), 0, 0));
			}
		}
		break;
	}
}
void Editor::on(MouseMoveEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(dragging){
		gi::cameraTargetX -= event.dx() / gi::dx();
		gi::cameraTargetY -= event.dy() / gi::dy();
	}
	if(!targeting){
		Target* nt;
		if((nt = world->drawableAt(float(event.x()), float(event.y()), selectedLayer)) != NULL){
			if(target != NULL){
				target->drawable->highlight = false;
				delete target;
			}
			target = nt;
			target->drawable->highlight = true;
		}
		else{
			if(target != NULL){
				target->drawable->highlight = false;
				delete target;
			}
			target = NULL;
		}
	}
	else{
		if(!dragging){
			if(target->layer == LAYER0){
				if(world->background != NULL){
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
			else if(manager->inputManager->isPressed(sf::Keyboard::Key::S)){
				FloatRect tr = target->drawable->getSprite(world->time())->sprite()->getGlobalBounds();
				for(drawable::Drawable* d : world->drawables[target->layer]){
					FloatRect dr = d->getSprite(world->time())->sprite()->getGlobalBounds();
					if(interv(dr.left + dr.width, tr.left) < SNAP){
						if(interv(dr.top + dr.height, tr.top) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
							break;
						}
						else if(interv(dr.top, tr.top) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy()) / gi::dy();
							break;
						}
						else if(interv(dr.top, tr.top + tr.height) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
							break;
						}
						else if(interv(dr.top + dr.height, tr.top + tr.height) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height + dr.height) / gi::dy();
							break;
						}
					}
					else if(interv(dr.left, tr.left + tr.width) < SNAP){
						if(interv(dr.top + dr.height, tr.top) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
							break;
						}
						else if(interv(dr.top, tr.top) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy()) / gi::dy();
							break;
						}
						else if(interv(dr.top, tr.top + tr.height) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
							break;
						}
						else if(interv(dr.top + dr.height, tr.top + tr.height) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height + dr.height) / gi::dy();
							break;
						}
					}
					else if(interv(dr.top + dr.height, tr.top) < SNAP){
						if(interv(dr.left, tr.left) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx()) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
							break;
						}
						else if(interv(dr.left + dr.width, tr.left + tr.width) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() + dr.height) / gi::dy();
							break;
						}
					}
					else if(interv(dr.top, tr.top + tr.height) < SNAP){
						if(interv(dr.left, tr.left) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx()) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
							break;
						}
						else if(interv(dr.left + dr.width, tr.left + tr.width) < SNAP){
							target->drawable->position.x = (d->position.x * gi::dx() + dr.width - tr.width) / gi::dx();
							target->drawable->position.y = (d->position.y * gi::dy() - tr.height) / gi::dy();
							break;
						}
					}
				}
			}
			else{
				target->drawable->position.x = gi::cameraX - gi::WIDTH / 2 + (event.x() - target->dx) / gi::dx();
				target->drawable->position.y = gi::cameraY - gi::HEIGHT / 2 + (event.y() - target->dy) / gi::dy();
			}
			target->drawable->updateOrder = true;
		}
	}
}
void Editor::on(MouseWheelEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(event.delta() > 0){
		selectedLayer = nextLayer(selectedLayer);
	}
	else{
		selectedLayer = previousLayer(selectedLayer);
	}
	if(target != NULL && targeting){
		drawable::Drawable* d = target->drawable;

		world->drawables[target->layer].erase(remove(world->drawables[target->layer].begin(), world->drawables[target->layer].end(), d));
		world->addDrawable(d, selectedLayer);
		target->layer = selectedLayer;
	}
	else{
		targeting = false;
		if(target != NULL){
			target->drawable->highlight = false;
			delete target;
			target = NULL;
		}
	}
	layerMenu->title = layerToString(selectedLayer);
	on(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
}