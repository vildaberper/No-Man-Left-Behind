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

	Brush* b = new Brush();
	for(int i = 1; i < 6; i++){
		b->objects.push_back("Big Trees.Bigtree " + std::to_string(i));
		b->objects.push_back("Big Trees.Bigtree " + std::to_string(i));
		b->objects.push_back("Big Trees.Bigtree " + std::to_string(i));
	}
	for(int i = 1; i < 6; i++){
		b->objects.push_back("Small Props.Stone " + std::to_string(i));
	}
	for(int i = 1; i < 6; i++){
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
		b->objects.push_back("Big Props.Pine " + std::to_string(i));
	}
	brushes["Forest"] = b;
	brush = "Forest";
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

		if(manager->inputManager->isPressed(sf::Keyboard::Key::Delete)){
			bool first = false;
			if(!deleting){
				deleting = true;
				deleteClock.restart();
				first = true;
			}
			if((first || deleteClock.getElapsedTime() >= deleteTime) && target != NULL){
				target->drawable->kill();
				delete target;
				target = NULL;
				targeting = false;
			}
			on(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
		}
		else{
			deleting = false;
		}

		if(usingBrush && draggingLeft){
			Brush* b = brushes[brush];
			for(int i = 0; i < max(1, int(brushRadius / 1000)); i++){
				b->paint(
					gi::wx(float(manager->inputManager->mouseX())),
					gi::wy(float(manager->inputManager->mouseY())),
					brushRadius,
					brushDensity,
					manager,
					world,
					selectedLayer
					);
			}
		}

		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		gi::camera(world->dt());

		world->render();

		if(usingBrush){
			CircleShape cs;
			cs.setPosition(float(manager->inputManager->mouseX()), float(manager->inputManager->mouseY()));
			cs.setRadius(brushRadius);
			cs.setOrigin(brushRadius, brushRadius);
			cs.scale(gi::dx(), gi::dy());
			cs.setFillColor(Color(0, 0, 0, 15));
			cs.setOutlineColor(Color(255, 0, 0, 255));
			cs.setOutlineThickness(3.0f / std::min(gi::dx(), gi::dy()));
			cs.setPointCount(size_t(std::max(10.0f, brushRadius / 4.5f)));
			gi::renderWindow->draw(cs);
		}

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
		case Keyboard::B:
			if(event.first()){
				if(usingBrush = !usingBrush){
					if(target != NULL){
						target->drawable->highlight = false;
						delete target;
						target = NULL;
					}
					targeting = false;
					logger::info("Using brush: " + brush);
				}
				else{
					on(MouseMoveEvent(manager->inputManager->mouseX(), manager->inputManager->mouseY(), 0, 0));
				}
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
		draggingRight = event.pressed();
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
		draggingLeft = event.pressed();
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
	if(draggingRight){
		gi::cameraTargetX -= event.dx() / gi::dx();
		gi::cameraTargetY -= event.dy() / gi::dy();
	}
	if(!targeting){
		if(!usingBrush){
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
					target = NULL;
				}
			}
			if(target == NULL && selectedLayer == LAYER0 && manager->inputManager->isPressed(Keyboard::N)){
				on(MouseButtonEvent(Mouse::Left, true, false, event.x(), event.y()));
				targeting = false;
			}
		}
	}
	else{
		if(target->layer == LAYER0){
			if(world->background != NULL){
				int w = world->background->getSize().x;
				int h = world->background->getSize().y;
				float x = gi::wx(float(event.x())) - float(w);
				float y = gi::wy(float(event.y())) - float(h);
				x = ceil(x / w) * w;
				y = ceil(y / h) * h;
				target->drawable->position.x = x;
				target->drawable->position.y = y;
				return;
			}
		}

		target->drawable->position.x = gi::cameraX - gi::WIDTH / 2 + (event.x() - target->dx) / gi::dx();
		target->drawable->position.y = gi::cameraY - gi::HEIGHT / 2 + (event.y() - target->dy) / gi::dy();

		if(manager->inputManager->isPressed(sf::Keyboard::Key::S)){
			FloatRect tr = target->drawable->getSprite(world->time())->sprite()->getGlobalBounds();
			size_t min = world->binarySearchRenderOffset(target->drawable->renderOffset() - MAX_COLLISION_DISTANCE, selectedLayer);
			size_t max = std::min(world->binarySearchRenderOffset(target->drawable->renderOffset() + MAX_COLLISION_DISTANCE, selectedLayer), world->drawables[selectedLayer].size() - 1);

			for(size_t i = min; i <= max; i++){
				drawable::Drawable* d = world->drawables[selectedLayer][i];
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
		target->drawable->updateOrder = true;
	}
}
void Editor::on(MouseWheelEvent& event){
	if(event.isCancelled()){
		return;
	}
	if(usingBrush){
		brushRadius += brushRadius * 0.1f * event.delta();
		if(brushRadius < 100.0f){
			brushRadius = 100.0f;
		}
		else if(brushRadius > 50000.0f){
			brushRadius = 50000.0f;
		}
	}
	else{
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
}