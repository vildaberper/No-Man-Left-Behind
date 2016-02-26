#include "World.h"

World::World(Manager* manager){
	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		Layer l = Layer(i);
		drawables[l] = std::vector<drawable::Drawable*>();
		doSwap[l] = false;
	}

	World::manager = manager;
}

World::~World(){

}

void World::setPaused(const bool& paused){
	World::paused = paused;
}
bool World::isPaused(){
	return paused;
}

void World::setTimeScale(const float& timeScale){
	World::timeScale = timeScale;
}
float World::getTimeScale(){
	return timeScale;
}

const sf::Time World::time(){
	return t;
}

const float World::dt(){
	return paused ? 0.0f : dt_.asSeconds();
}

void World::tick(){
	if(firstTick){
		clock.restart();
		firstTick = false;
	}

	if(hasRendered){
		for(unsigned int i = 0; i < NUM_LAYERS; i++){
			Layer l = Layer(i);
			if(doSwap[l]){
				orderDrawables(l);
			}
		}
	}

	dt_ = clock.restart() * getTimeScale();

	if(paused){
		return;
	}

	t += dt_;

	for(Entity* e : entities){
		e->tick(time(), dt());
	}

	bool foundX = false;
	bool foundY = false;
	bool foundXY = false;
	for(drawable::Drawable* d0 : collidables){
		if(d0->velocity.direction() == ZERO){
			continue;
		}
		foundX = false;
		foundY = false;
		foundXY = false;

		for(drawable::Drawable* d1 : collidables){
			if(d0 == d1){
				continue;
			}
			if(math::interv(d0->position.x, d1->position.x) + math::interv(d0->position.y, d1->position.y) > MAX_COLLISION_DISTANCE){
				continue;
			}
			if(d0->collidesWith(d1, time(), d0->position + Vector(d0->velocity.x * dt(), 0.0f))){
				foundX = true;
			}
			if(d0->collidesWith(d1, time(), d0->position + Vector(0.0f, d0->velocity.y * dt()))){
				foundY = true;
			}
			if(d0->collidesWith(d1, time(), d0->position + (d0->velocity * dt()))){
				foundXY = true;
			}
		}

		if(foundX || (!foundY && foundXY)){
			d0->velocity.x = 0.0f;
		}
		if(foundY || (!foundX && foundXY)){
			d0->velocity.y = 0.0f;
		}
	}

	for(Entity* e : entities){
		e->move(dt());
	}

	cleanAll(false);
}


const void World::renderBackground(){
	if(background != NULL){
		sf::Sprite s = sf::Sprite(*background, sf::IntRect(0, 0, int(gi::WIDTH + 3 * background->getSize().x), int(gi::HEIGHT + 3 * background->getSize().y)));
		float x = (-gi::cameraX + gi::WIDTH / 2) * gi::dx();
		float y = (-gi::cameraY + gi::HEIGHT / 2) * gi::dy();
		x = fmod(x, background->getSize().x * gi::dx());
		y = fmod(y, background->getSize().y * gi::dy());
		s.setPosition(x - background->getSize().x * gi::dx(), y - background->getSize().y * gi::dy());
		s.scale(gi::dx(), gi::dy());
		gi::draw(s);
	}
}
const void World::render(){
	renderBackground();
	for(const auto &ent : drawables){
		for(drawable::Drawable* d : ent.second){
			gi::draw(d, time());
		}
	}
	hasRendered = true;
}
const void World::render(drawable::Drawable* relative){
	renderBackground();
	std::vector<drawable::Drawable*> under;
	std::vector<drawable::Drawable*> above;

	sf::FloatRect rel = relative->getSprite(time())->getGlobalBounds();
	for(drawable::Drawable* d : drawables[LAYER2]){
		if(d == relative){
			continue;
		}
		sf::FloatRect fr = d->getSprite(time())->getGlobalBounds();
		if(fr.top + fr.height * d->cb.renderOffset <= rel.top + rel.height * relative->cb.renderOffset){
			under.push_back(d);
		}
		else{
			above.push_back(d);
		}
	}

	for(const auto &ent : drawables){
		if(ent.first == LAYER2){
			break;
		}
		for(drawable::Drawable* d : ent.second){
			gi::draw(d, time());
		}
	}
	for(drawable::Drawable* d : under){
		gi::draw(d, time());
	}
	gi::draw(relative, time());
	for(drawable::Drawable* d : above){
		gi::draw(d, time());
	}
	for(const auto &ent : drawables){
		if(ent.first == LAYER0 || ent.first == LAYER1 || ent.first == LAYER2){
			continue;
		}
		for(drawable::Drawable* d : ent.second){
			gi::draw(d, time());
		}
	}
	hasRendered = true;
}

bool World::isOrdering(){
	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		if(doSwap[Layer(i)]){
			return true;
		}
	}
	return false;
}

void World::orderDrawables(const Layer& layer){
	bool swapped;
	unsigned int ss = 0;
	drawable::Drawable* current;
	drawable::Drawable* before;

	do{ // Yay bubblesort TODO real time ordering by index
		swapped = false;
		for(size_t i = 1; i < drawables[layer].size() && ss < MAX_SWAPS_PER_FRAME; i++){
			if(*(before = drawables[layer][i - 1]) > *(current = drawables[layer][i])){
				drawables[layer][i] = before;
				drawables[layer][i - 1] = current;
				swapped = true;
				swapPosition[layer] = i;
				ss++;
			}
		}
	} while(swapped && ss < MAX_SWAPS_PER_FRAME);
	if(doSwap[layer] = ss >= MAX_SWAPS_PER_FRAME - 1){
		swapLayer = layer;
	}
}

void World::addDrawable(drawable::Drawable* drawable, const Layer& layer){
	entities.push_back((Entity*) drawable);
	drawables[layer].push_back(drawable);

	drawable->reference = drawable->animations[drawable->currentAnimation]->textures[0];
	if(!drawable->cb.shouldCollide){
		drawable->cb = manager->collisionManager->getCollisionBox(drawable->reference);
	}
	if(drawable->cb.shouldCollide){
		collidables.push_back(drawable);
	}
	doSwap[layer] = true;
}

Target* World::drawableAt(const float& x, const float& y, const Layer& layer){
	float rwx = gi::cameraX - gi::WIDTH / 2 + x / gi::dx();
	float rwy = gi::cameraY - gi::HEIGHT / 2 + y / gi::dy();
	if(drawables[layer].size() > 0){
		for(size_t i = 0; i < drawables[layer].size(); i++){
			drawable::Drawable* d = drawables[layer][drawables[layer].size() - i - 1];

			if(math::interv(rwx, d->position.x) + math::interv(rwy, d->position.y) > MAX_COLLISION_DISTANCE){
				continue;
			}

			sf::FloatRect fr = d->getSprite(time())->getGlobalBounds();
			if(
				fr.left < x
				&& fr.left + fr.width > x
				&& fr.top < y
				&& fr.top + fr.height > y
				){
				return new Target(
					d,
					layer,
					x - fr.left,
					y - fr.top
					);
			}
		}
	}
	return NULL;
}

void save_helper(Configuration& c, std::vector<drawable::Drawable*>& ds, std::string layer){
	for(size_t i = 0; i < ds.size(); i++){
		drawable::Drawable d = *ds[i];
		if(!d.isAlive()){
			continue;
		}
		std::string sub = "drawables." + layer + "." + std::to_string(i);
		c.set(sub + ".currentAnimation", d.currentAnimation);
		c.set(sub + ".nextAnimation", d.nextAnimation);
		c.set(sub + ".scale", d.scale);
		c.set(sub + ".health", d.health);
		c.set(sub + ".position", d.position.fv());
		c.set(sub + ".velocity", d.velocity.fv());
		sub += ".animations";
		for(auto &ent : d.animations){
			drawable::Animation* a = d.animations[ent.first];
			std::string suba = sub + "." + ent.first;
			c.set(suba + ".timing", a->timing.asMilliseconds());
			c.set(suba + ".textures", a->textures);
		}
	}
}

void World::save(File& f){
	sf::Clock cl;
	Configuration c;

	if(backgroundName.length() > 0){
		c.set("background", backgroundName);
	}
	save_helper(c, drawables[LAYER0], "LAYER0");
	save_helper(c, drawables[LAYER1], "LAYER1");
	save_helper(c, drawables[LAYER2], "LAYER2");
	save_helper(c, drawables[LAYER3], "LAYER3");
	save_helper(c, drawables[LAYER4], "LAYER4");
	c.save(f);
	logger::timing("World configuration saved in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	logger::info("World saved: " + f.parent().name() + "\\" + f.name());
}

unsigned int load_helper(Configuration& c, World* w, std::string layer, Manager* m){
	std::vector<std::string> cs = c.children("drawables." + layer);
	size_t size = cs.size();
	for(size_t i = 0; i < size; i++){
		std::string sub = "drawables." + layer + "." + std::to_string(i);
		drawable::Drawable* d = new drawable::Drawable();
		d->currentAnimation = c.stringValue(sub + ".currentAnimation");
		d->nextAnimation = c.stringValue(sub + ".nextAnimation");
		d->scale = c.floatValue(sub + ".scale");
		d->health = c.floatValue(sub + ".health");
		d->position = Vector(c.floatVector(sub + ".position"));
		d->position.x = ceil(d->position.x);
		d->position.y = ceil(d->position.y);
		d->velocity = Vector(c.floatVector(sub + ".velocity"));
		sub += ".animations";
		for(std::string an : c.children(sub, false)){
			drawable::Animation* a = new drawable::Animation();
			a->timing = sf::milliseconds(c.intValue(sub + "." + an + ".timing"));
			a->textures = c.stringVector(sub + "." + an + ".textures");
			for(std::string t : a->textures){
				a->sprites.push_back(m->spriteManager->getSprite(t));
			}
			d->animations[an] = a;
		}
		w->addDrawable(d, parseLayer(layer));
	}
	return size;
}

void World::load(File& f){
	cleanAll(true);

	sf::Clock cl;
	Configuration c;

	c.load(f);
	logger::timing("World configuration loaded in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	cl.restart();
	backgroundName = c.stringValue("background");
	unsigned int total = 0;
	total += load_helper(c, this, "LAYER0", manager);
	total += load_helper(c, this, "LAYER1", manager);
	total += load_helper(c, this, "LAYER2", manager);
	total += load_helper(c, this, "LAYER3", manager);
	total += load_helper(c, this, "LAYER4", manager);
	logger::timing(std::to_string(total) + " objects added in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	logger::info("World loaded: " + f.parent().name() + "\\" + f.name());
}

void World::cleanAll(const bool& all){
	for(auto &ent : drawables){
		ent.second.erase(
			remove_if(ent.second.begin(), ent.second.end(),
				[all](drawable::Drawable* e){ return all || !((Entity*) e)->isAlive(); }),
			ent.second.end());
	}

	for(size_t i = 0; i < entities.size(); i++){
		if(all || !entities[i]->isAlive()){
			// delete entities[i]; TODO Fix memory leak
			entities.erase(entities.begin() + (i--));
		}
	}
}