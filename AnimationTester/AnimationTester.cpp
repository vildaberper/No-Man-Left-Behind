#include "AnimationTester.h"

using namespace sf;
using namespace std;

AnimationTester* animationTester;

void keyboardListenerW(KeyboardEvent& event){
	animationTester->keyboardListener(event);
}
void mouseWheelListenerW(MouseWheelEvent& event){
	animationTester->mouseWheelListener(event);
}

AnimationTester::AnimationTester(){
	animationTester = this;
}

AnimationTester::~AnimationTester(){

}

void AnimationTester::run(){
	Clock fg;
	c::initialize();
	logger::timing("Constants initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	fg.restart();
	gi::initalize(window);
	logger::timing("Graphics interface initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");
	manager = new Manager();
	manager->initialize(window);
	logger::timing("Manager initialized in " + std::to_string(fg.getElapsedTime().asSeconds()) + " seconds.");

	manager->inputManager->registerListener(keyboardListenerW);
	manager->inputManager->registerListener(mouseWheelListenerW);

	world = new World();

	load();

	window->setFramerateLimit(60);
	while (gi::startOfFrame()){
		world->tick();
		manager->tick(window, world->time(), world->dt());

		window->clear();

		world->render();
		manager->menuManager->draw(world->time());

		gi::endOfFrame();
	}

	save();

	manager->finalize(window);
	delete manager;
	gi::finalize();
	delete world;
}

void AnimationTester::load(){
	pngFile = File().child("animation.png");
	txtFile = File().child("animation.txt");
	Configuration c;
	Texture* t = new Texture();
	if (!c.load(txtFile) || !t->loadFromFile(pngFile.path())){
		logger::fatal("Could not read animaion.txt and/or animation.png");
		return;
	}

	if (world != NULL){
		delete world;
	}
	if (d != NULL){
		logger::warning("Reloading");
		delete d;
	}
	if (a != NULL){
		delete a;
	}
	world = new World();

	TextureManager* tm = manager->spriteManager->texMan;
	tm->textures.insert(t);

	TexI* ti = new TexI();
	ti->texture = t;
	ti->width = c.intVector("textures")[0];
	ti->height = c.intVector("textures")[1];

	a = new drawable::Animation();
	a->timing = sf::milliseconds(c.intValue("timing"));

	drawable::Animation* a0 = new drawable::Animation();
	a0->timing = sf::milliseconds(100);

	int frame = 0;
	for (unsigned char y = 0; y < ti->height; y++){
		for (unsigned char x = 0; x < ti->width; x++){
			string f = to_string(frame++);
			SubTexture* st = new SubTexture();
			st->texi = ti;
			st->x = x;
			st->y = y;
			tm->textureMap["animation"][f] = st;
			a->sprites.push_back(manager->spriteManager->getSprite("animation", f));
			a->textures.push_back("animation." + f);

			if (x == 0 && y == 0){
				a0->sprites.push_back(manager->spriteManager->getSprite("animation", f));
				a0->textures.push_back("animation." + f);
			}
		}
	}

	d = new drawable::Drawable();
	d->animations["animation"] = a;
	d->animations["idle"] = a0;
	d->currentAnimation = d->nextAnimation = "animation";
	d->position.x = 0;
	d->position.y = 0;
	world->addDrawable(d, LAYER1);

	if (!repeat){
		d->nextAnimation = "idle";
	}
}

void AnimationTester::save(){
	Configuration c;
	if (a == NULL){
		logger::warning("There is no timing to save!");
		return;
	}
	if (!c.load(txtFile)){
		logger::fatal("Could not read animaion.txt");
		return;
	}
	c.set("timing", a->timing.asMilliseconds());
	if (!c.save(txtFile)){
		logger::fatal("Could not save animaion.txt");
		return;
	}
	logger::info("Saved");
}

const void AnimationTester::keyboardListener(KeyboardEvent& event){
	if (event.isCancelled()){
		return;
	}
	if (event.pressed()){
		switch (event.key()){
		case Keyboard::Escape:
			window->close();
			break;
		case Keyboard::S:
			if (manager->inputManager->isPressed(Keyboard::LControl)){
				save();
			}
			break;
		case Keyboard::F5:
			load();
			break;
		case Keyboard::P:
			if (d != NULL && event.first()){
				if (repeat){
					if (d->nextAnimation == "idle"){
						d->nextAnimation = "animation";
					}
					else{
						d->nextAnimation = "idle";
					}
					logger::info("State: " + d->nextAnimation);
				}
				else{
					d->currentAnimation = "animation";
					d->startTime = world->time();
				}
			}
			break;
		case Keyboard::R:
			if (!(repeat = !repeat)){
				d->nextAnimation = "idle";
			}
			logger::info("Repeating: " + string(repeat ? "yes" : "no"));
			break;
		case Keyboard::Add:
			if (a != NULL){
				if (manager->inputManager->isPressed(sf::Keyboard::Key::LShift)){
					a->timing += sf::milliseconds(10);
				}
				else{
					a->timing += sf::milliseconds(1);
				}
				if (a->timing < milliseconds(1)){
					a->timing = milliseconds(1);
				}
				logger::info("Timing: " + to_string(a->timing.asMilliseconds()));
			}
			break;
		case Keyboard::Subtract:
			if (a != NULL){
				if (manager->inputManager->isPressed(sf::Keyboard::Key::LShift)){
					a->timing -= sf::milliseconds(10);
				}
				else{
					a->timing -= sf::milliseconds(1);
				}
				if (a->timing < milliseconds(1)){
					a->timing = milliseconds(1);
				}
				logger::info("Timing: " + to_string(a->timing.asMilliseconds()));
			}
			break;
		}
	}
}

const void AnimationTester::mouseWheelListener(MouseWheelEvent& event){
	if (event.isCancelled()){
		return;
	}
	if (d != NULL){
		if (event.delta() > 0){
			d->scale *= 1.2f;
		}
		else{
			d->scale *= (5.0f / 6.0f);
		}
		if (d->scale < 1.0f){
			d->scale = 1.0f;
		}
		logger::info("Scale: " + to_string(d->scale));
	}
}