#include "CollisionEditor.h"

using namespace sf;
using namespace std;

CollisionEditor::CollisionEditor(){
	current = new std::string();
	selectedString = new std::string();
}

CollisionEditor::~CollisionEditor(){
	delete current;
	delete selectedString;
}

void CollisionEditor::run(){
	c::initialize();
	c::showLog = true;
	gi::initalize();
	manager = new Manager();
	manager->initialize(gi::renderWindow);
	world = new World(manager);
	world->background = manager->spriteManager->getBackground("Snow");

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

	manager->inputManager->registerListener(this);

	gi::collisionBoxes = true;

	while(gi::startOfFrame()){
		if(manager->inputManager->isPressed(sf::Keyboard::Key::Z)){
			gi::zoom(gi::cameraZ + 1.0f * gi::cameraZ * world->dt());
		}
		if(manager->inputManager->isPressed(sf::Keyboard::Key::X)){
			gi::zoom(gi::cameraZ - 1.0f * gi::cameraZ * world->dt());
		}

		if(current->length() > 0){
			CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);

			if(!manager->inputManager->isPressed(Keyboard::LControl)){
				if(manager->inputManager->isPressed(Keyboard::LShift)){
					if(manager->inputManager->isPressed(Keyboard::W)){
						cb->size.y -= 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::S)){
						cb->size.y += 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::A)){
						cb->size.x -= 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::D)){
						cb->size.x += 0.1f * world->dt();
					}
				}
				else{
					if(manager->inputManager->isPressed(Keyboard::W)){
						cb->offset.y -= 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::S)){
						cb->offset.y += 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::A)){
						cb->offset.x -= 0.1f * world->dt();
					}
					if(manager->inputManager->isPressed(Keyboard::D)){
						cb->offset.x += 0.1f * world->dt();
					}
				}
			}
			update();
		}

		world->tick();
		manager->tick(gi::renderWindow, world->time(), world->dt());

		world->render();

		gi::drawLog();

		manager->menuManager->draw(world->time());
		gi::endOfFrame();
	}
	delete world;
	manager->collisionManager->save(c::collisionBoxFile);
	manager->finalize(gi::renderWindow);
	delete manager;
	gi::finalize();
}

void CollisionEditor::update(){
	if(d != nullptr){
		d->kill();
	}

	if(current->length() == 0){
		return;
	}

	CoreSprite* s = manager->spriteManager->getSprite(*current);
	d = new drawable::Drawable();
	drawable::Animation* a = new drawable::Animation();
	d->position = Vector(
		gi::TARGET_WIDTH / 2 - s->sprite()->getGlobalBounds().width / 2,
		gi::TARGET_HEIGHT / 2 - s->sprite()->getGlobalBounds().height / 2
		);
	d->scale = 1.0f;
	a->textures.push_back(*current);
	a->sprites.push_back(s);
	a->timing = milliseconds(1000);
	d->animations["default"] = a;
	d->currentAnimation = d->nextAnimation = "default";
	world->addDrawable(d, LAYER2);
}

void CollisionEditor::on(KeyboardEvent& event){
	if(event.isCancelled()){
		return;
	}
	switch(event.key()){
	case Keyboard::Escape:
		gi::renderWindow->close();
		break;
	case Keyboard::C:
	{
		if(event.pressed() && event.first()){
			if(current->length() > 0){
				CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);
				cb->shouldCollide = !cb->shouldCollide;
				update();
			}
		}
		break;
	}
	case Keyboard::L:
		if(event.first()){
			gi::logAlwaysActive = !gi::logAlwaysActive;
		}
		break;
	case Keyboard::S:
		if(event.first()){
			if(manager->inputManager->isPressed(Keyboard::LControl)){
				manager->collisionManager->save(c::collisionBoxFile);
			}
		}
		break;
	}
}
void CollisionEditor::on(MouseButtonEvent& event){
	if(event.isCancelled()){
		if(event.button() == Mouse::Button::Left && event.pressed()){
			if(selectedString->length() > 0){
				if(manager->inputManager->isPressed(Keyboard::LShift)){
					if(current->length() > 0 && d != nullptr){
						CollisionBox* cbc = manager->collisionManager->getCollisionBoxReference(*selectedString);
						CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);
						cb->offset = cbc->offset;
						cb->renderOffset = cbc->renderOffset;
						cb->shouldCollide = cbc->shouldCollide;
						cb->size = cbc->size;
					}
				}
				else{
					delete current;
					current = new string(*selectedString);
				}
				update();
			}
		}
		return;
	}
	switch(event.button()){
	case Mouse::Button::Left:
	{
		if(event.pressed()){
			if(current->length() > 0 && d != nullptr){
				FloatRect fr = d->getSprite(world->time())->sprite()->getGlobalBounds();
				CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);

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
	case Mouse::Button::Right:
	{
		if(event.pressed()){
			if(current->length() > 0 && d != nullptr){
				FloatRect fr = d->getSprite(world->time())->sprite()->getGlobalBounds();
				CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);

				float y = (event.y() - fr.top) / fr.height;

				cb->renderOffset = y;

				update();
			}
		}
		break;
	}
	}
}

void CollisionEditor::on(MouseMoveEvent& event){
	if(dragging){
		if(current->length() > 0 && d != nullptr){
			FloatRect fr = d->getSprite(world->time())->sprite()->getGlobalBounds();
			CollisionBox* cb = manager->collisionManager->getCollisionBoxReference(*current);

			float x = (event.x() - fr.left - cb->offset.x * fr.width) / fr.width;
			float y = (event.y() - fr.top - cb->offset.y * fr.height) / fr.height;

			cb->size.x = x;
			cb->size.y = y;

			update();
		}
	}
}