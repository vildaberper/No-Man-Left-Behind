#include "World.h"

World::World(){
	drawables[LAYER0] = std::vector<drawable::Drawable*>();
	drawables[LAYER1] = std::vector<drawable::Drawable*>();
	drawables[LAYER2] = std::vector<drawable::Drawable*>();
	drawables[LAYER3] = std::vector<drawable::Drawable*>();
	drawables[LAYER4] = std::vector<drawable::Drawable*>();
	lastTime = clock.getElapsedTime();
}

World::~World(){

}

void World::tick(){
	sf::Time time = clock.getElapsedTime();
	dt_ = (time - lastTime).asSeconds();

	for (Entity* e : entities){
		e->tick(time, dt_);
	}

	lastTime = time;
	cleanAll(false);
}

const void World::render(){
	for (const auto &ent : drawables){
		for (drawable::Drawable* d : ent.second){
			gi::draw(d, lastTime);
		}
	}
}

const sf::Time World::time(){
	return lastTime;
}

const float World::dt(){
	return dt_;
}

void World::addDrawable(drawable::Drawable* drawable, const Layer& layer){
	entities.push_back((Entity*) drawable);
	drawables[layer].push_back(drawable);
}

Target* World::drawableAt(const float& x, const float& y, const Layer& layer){
	if (drawables[layer].size() > 0){
		for (size_t i = 0; i < drawables[layer].size(); i++){
			sf::FloatRect fr = drawables[layer][drawables[layer].size() - i - 1]->getSprite(lastTime)->getGlobalBounds();
			if (fr.contains(sf::Vector2f(x, y))){
				return new Target(drawables[layer][drawables[layer].size() - i - 1], layer, x - fr.left, y - fr.top);
			}
		}
	}
	return NULL;
}

void save_helper(Configuration& c, std::vector<drawable::Drawable*>& ds, std::string layer){
	for (size_t i = 0; i < ds.size(); i++){
		drawable::Drawable d = *ds[i];
		if (!d.isAlive()){
			continue;
		}
		std::string sub = layer + "." + std::to_string(i);
		c.set(sub + ".currentAnimation", d.currentAnimation);
		c.set(sub + ".nextAnimation", d.nextAnimation);
		c.set(sub + ".scale", d.scale);
		c.set(sub + ".rotation", d.rotation);
		c.set(sub + ".health", d.health);
		c.set(sub + ".position", d.position.fv());
		c.set(sub + ".velocity", d.velocity.fv());
		sub += ".animations";
		for (auto &ent : d.animations){
			drawable::Animation a = d.animations[ent.first];
			std::string suba = sub + "." + ent.first;
			c.set(suba + ".timing", a.timing.asMilliseconds());
			c.set(suba + ".textures", a.textures);
		}
	}
}

void World::save(File& f){
	Configuration c;

	for (std::string s : c.children("")){
		logger::warning("config error (save) " + s);
		c.remove(s);
	}
	save_helper(c, drawables[LAYER0], "LAYER0");
	save_helper(c, drawables[LAYER1], "LAYER1");
	save_helper(c, drawables[LAYER2], "LAYER2");
	save_helper(c, drawables[LAYER3], "LAYER3");
	save_helper(c, drawables[LAYER4], "LAYER4");
	c.save(f);
	logger::info("World saved: " + f.path());
}

void load_helper(Configuration& c, World* w, std::string layer, Manager* m){
	std::vector<std::string> cs = c.children(layer);
	size_t size = cs.size();
	for (size_t i = 0; i < size; i++){
		std::string sub = layer + "." + std::to_string(i);
		drawable::Drawable* d = new drawable::Drawable();
		d->currentAnimation = c.stringValue(sub + ".currentAnimation");
		d->nextAnimation = c.stringValue(sub + ".nextAnimation");
		d->scale = c.floatValue(sub + ".scale");
		d->rotation = c.floatValue(sub + ".rotation");
		d->health = c.floatValue(sub + ".health");
		d->position = Vector(c.floatVector(sub + ".position"));
		d->position.x = ceil(d->position.x);
		d->position.y = ceil(d->position.y);
		d->velocity = Vector(c.floatVector(sub + ".velocity"));
		sub += ".animations";
		for (std::string an : c.children(sub, false)){
			drawable::Animation a;
			a.timing = sf::milliseconds(c.intValue(sub + "." + an + ".timing"));
			a.textures = c.stringVector(sub + "." + an + ".textures");
			for (std::string t : a.textures){
				a.sprites.push_back(m->spriteManager->getSprite(t));
			}
			d->animations[an] = a;
		}
		w->addDrawable(d, parseLayer(layer));
	}
}

void World::load(File& f, Manager* m){
	cleanAll(true);

	Configuration c;

	for (std::string s : c.children("")){
		logger::warning("config error (load) " + s);
		c.remove(s);
	}
	c.load(f);
	load_helper(c, this, "LAYER0", m);
	load_helper(c, this, "LAYER1", m);
	load_helper(c, this, "LAYER2", m);
	load_helper(c, this, "LAYER3", m);
	load_helper(c, this, "LAYER4", m);
	logger::info("World loaded: " + f.path());
}

void World::cleanAll(const bool& all){
	for (auto &ent : drawables){
		ent.second.erase(
			remove_if(ent.second.begin(), ent.second.end(),
			[all](drawable::Drawable* e){ return all || !((Entity*) e)->isAlive(); }),
			ent.second.end());
	}

	for (size_t i = 0; i < entities.size(); i++){
		if (all || !entities[i]->isAlive()){
			/*
			TODO
			
			FIX POTENTIAL MEMORY LEAK
			*/
			//delete entities[i];
			entities.erase(entities.begin() + (i--));
		}
	}
}