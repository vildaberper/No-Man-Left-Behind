#include "CollisionEditor.h"

using namespace sf;
using namespace std;

CollisionEditor* collisionEditor;

void keyboardListenerW(KeyboardEvent& event){
	collisionEditor->keyboardListener(event);
}
void mouseButtonListenerW(MouseButtonEvent& event){
	collisionEditor->mouseButtonListener(event);
}
void mouseMoveListenerW(MouseMoveEvent& event){
	collisionEditor->mouseMoveListener(event);
}

CollisionEditor::CollisionEditor(){
	collisionEditor = this;
	selectedString = new std::string();
}

CollisionEditor::~CollisionEditor(){
	delete selectedString;
}

void CollisionEditor::run(){
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

	gi::collisionBoxes = true;

	window->setFramerateLimit(60);
	while(gi::startOfFrame()){
		if(manager->inputManager->isPressed(sf::Keyboard::Key::Z)){
			gi::zoom(gi::cameraZ + 1.0f * gi::cameraZ * world->dt());
		}
		if(manager->inputManager->isPressed(sf::Keyboard::Key::X)){
			gi::zoom(gi::cameraZ - 1.0f * gi::cameraZ * world->dt());
		}

		// Keyboard
		if(manager->inputManager->isPressed(Keyboard::LShift)){
			if(manager->inputManager->isPressed(Keyboard::W)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->size.y -= 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::S)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->size.y += 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::A)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->size.x -= 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::D)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->size.x += 0.1f * world->dt();
					update();
				}
			}
		}
		else{
			if(manager->inputManager->isPressed(Keyboard::W)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->offset.y -= 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::S)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->offset.y += 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::A)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->offset.x -= 0.1f * world->dt();
					update();
				}
			}
			if(manager->inputManager->isPressed(Keyboard::D)){
				if(selectedString->length() > 0){
					CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
					cb->offset.x += 0.1f * world->dt();
					update();
				}
			}
		}

		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		world->render();

		manager->menuManager->draw(world->time());

		gi::endOfFrame();
	}
	manager->collisionManager->save(c::collisionBoxFile);
	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
}

void CollisionEditor::update(){
	if(d != NULL){
		d->kill();
	}

	if(selectedString->length() == 0){
		return;
	}

	Sprite* s = manager->spriteManager->getSprite(*selectedString);
	d = new drawable::Drawable();
	drawable::Animation* a = new drawable::Animation();
	d->position = Vector(
		gi::TARGET_WIDTH / 2 - s->getGlobalBounds().width / 2,
		gi::TARGET_HEIGHT / 2 - s->getGlobalBounds().height / 2
		);
	d->scale = 1.0f;
	a->textures.push_back(*selectedString);
	a->sprites.push_back(s);
	a->timing = milliseconds(1000);
	d->animations["default"] = a;
	d->currentAnimation = d->nextAnimation = "default";
	world->addDrawable(d, LAYER2);
}

const void CollisionEditor::keyboardListener(KeyboardEvent& event){
	if(event.isCancelled()){
		return;
	}
	switch(event.key()){
	case Keyboard::C:
	{
		if(event.pressed() && event.first()){
			if(selectedString->length() > 0){
				CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);
				cb->shouldCollide = !cb->shouldCollide;
				update();
			}
		}
		break;
	}
	}
}

const void CollisionEditor::mouseButtonListener(MouseButtonEvent& event){
	if(event.isCancelled()){
		if(event.button() == Mouse::Button::Left && event.pressed()){
			if(selectedString->length() > 0){
				update();
			}
		}
		return;
	}
	switch(event.button()){
	case Mouse::Button::Left:
	{
		if(event.pressed()){
			if(selectedString->length() > 0 && d != NULL){
				FloatRect fr = d->getSprite(world->time())->getGlobalBounds();
				CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);

				cb->shouldCollide = true;

				float x = (event.x() - fr.left) / fr.width;
				float y = (event.y() - fr.top) / fr.height;

				cb->offset.x = x;
				cb->offset.y = y;

				dragging = true;
				update();
			}
		}
		else{
			dragging = false;
		}
		break;
	}
	}
}

const void CollisionEditor::mouseMoveListener(MouseMoveEvent& event){
	if(dragging){
		if(selectedString->length() > 0 && d != NULL){
			FloatRect fr = d->getSprite(world->time())->getGlobalBounds();
			CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*selectedString);

			float x = (event.x() - fr.left - cb->offset.x * fr.width) / fr.width;
			float y = (event.y() - fr.top - cb->offset.y * fr.height) / fr.height;

			cb->size.x = x;
			cb->size.y = y;

			update();
		}
	}
}