#include "World.h"

World::World(Manager* manager){
	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		Layer l = Layer(i);
		drawables[l] = std::vector<drawable::Drawable*>();
		doSwap[l] = true;
	}

	World::manager = manager;
}

World::~World(){
	cleanAll(true);
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

size_t World::numDrawables(){
	size_t total = 0;
	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		total += drawables[Layer(i)].size();
	}

	return total;
}

void World::tick(){
	cleanAll(false);
	if(firstTick){
		clock.restart();
		firstTick = false;
	}

	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		Layer l = Layer(i);
		if(doSwap[l]){
			orderDrawables(l);
		}
	}

	dt_ = clock.restart() * getTimeScale();

	if(dt_.asMicroseconds() == 0 || paused){
		return;
	}

	t += dt_;

	for(Entity* e : entities){
		e->tick(time(), dt());
	}

	bool foundX = false;
	bool foundY = false;
	bool foundXY = false;
	float smin = gi::cameraY - gi::HEIGHT / 2;
	float smax = gi::cameraY + gi::HEIGHT / 2;
	size_t d0min = binarySearchRenderOffset(smin - 1500.0f, LAYER2);
	size_t d0max = binarySearchRenderOffset(smax + 1500.0f, LAYER2);

	for(size_t di0 = d0min; di0 < drawables[LAYER2].size() && di0 <= d0max; di0++){
		drawable::Drawable* d0 = drawables[LAYER2][di0];

		if(d0->velocity.direction() == ZERO){
			continue;
		}

		size_t d1min = binarySearchRenderOffset(smin - MAX_COLLISION_DISTANCE, LAYER2);
		size_t d1max = binarySearchRenderOffset(smax + MAX_COLLISION_DISTANCE, LAYER2);

		for(size_t di1 = d1min; di1 < drawables[LAYER2].size() && di1 <= d0max; di1++){
			drawable::Drawable* d1 = drawables[LAYER2][di1];

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

			if(foundX && foundY && foundXY){
				continue;
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

	unsigned int numChecks = 0;
	for(; orderLayerI < NUM_LAYERS && numChecks < MAX_ORDERCHECKS_PER_FRAME;){
		Layer l = Layer(orderLayerI);
		for(; orderIndexI < drawables[l].size() && numChecks < MAX_ORDERCHECKS_PER_FRAME;){
			if(drawables[l][orderIndexI]->updateOrder){
				orderDrawable(orderIndexI, l);
			}
			else{
				orderIndexI++;
			}
			numChecks++;
		}
		if(drawables[l].size() == 0 || orderIndexI >= drawables[l].size()){
			orderIndexI = 0;
			orderLayerI++;
		}
	}
	if(orderLayerI >= NUM_LAYERS - 1){
		orderLayerI = 0;
	}
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
	float cymin = gi::cameraY - gi::HEIGHT / 2 - 1500.0f;
	float cymax = gi::cameraY + gi::HEIGHT / 2 + 1500.0f;
	for(unsigned int i = 0; i < NUM_LAYERS; i++){
		Layer l = Layer(i);
		size_t min = binarySearchRenderOffset(cymin, l);
		size_t max = binarySearchRenderOffset(cymax, l);
		for(size_t di = min; di < drawables[l].size() && di <= max; di++){
			gi::draw(drawables[l][di], time());
		}
	}
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
			if((before = drawables[layer][i - 1])->renderOffset() > (current = drawables[layer][i])->renderOffset()){
				drawables[layer][i] = before;
				drawables[layer][i - 1] = current;
				swapped = true;
				swapPosition[layer] = i;
				ss++;
			}
		}
	} while(swapped && ss < MAX_SWAPS_PER_FRAME);
	if(doSwap[layer] = ss != 0){
		swapLayer = layer;
	}
}

void World::insertDrawableAt(drawable::Drawable* drawable, const Layer& layer, const size_t& index){
	if(index >= drawables[layer].size()){
		drawables[layer].push_back(drawable);
		return;
	}

	drawable::Drawable* tmp0 = drawables[layer][index];
	drawable::Drawable* tmp1;

	for(size_t i = index + 1; i < drawables[layer].size(); i++){
		tmp1 = drawables[layer][i];
		drawables[layer][i] = tmp0;
		tmp0 = tmp1;
	}
	drawables[layer].push_back(tmp0);
	drawables[layer][index] = drawable;
}

size_t World::binarySearchRenderOffset(const float& co, const Layer& layer){
	size_t size = drawables[layer].size();

	if(size == 0){
		return 1;
	}

	size_t lo = 0;
	size_t hi = size - 1;
	size_t mi;

	float loo = drawables[layer][lo]->renderOffset();
	float hio = drawables[layer][hi]->renderOffset();

	if(co <= loo){
		return 0;
	}
	if(co >= hio){
		return drawables[layer].size();
	}
	size_t mini = size - min(size - 1, 20);
	for(size_t i = size - 1; i >= mini; i--){
		hi = i;
		hio = drawables[layer][hi]->renderOffset();
		if(co >= hio){
			return i + 1;
		}
	}

	float mio;

	while(lo < hi){
		mi = lo + (hi - lo) / 2;
		mio = drawables[layer][mi]->renderOffset();

		if(mio < co){
			lo = mi;
			loo = drawables[layer][lo]->renderOffset();
		}
		else{
			hi = mi;
			hio = drawables[layer][hi]->renderOffset();
		}
		if(hi - lo == 1){
			break;
		}
	}
	if(loo >= co){
		return lo;
	}
	if(mio >= co){
		return mi;
	}
	return hi;
}
size_t World::binarySearchRenderOffset(drawable::Drawable* drawable, const Layer& layer){
	return binarySearchRenderOffset(drawable->renderOffset(), layer);
}

void World::orderDrawable(const size_t& index, const Layer& layer){
	drawables[layer][index]->updateOrder = false;
	float co = drawables[layer][index]->renderOffset();
	drawable::Drawable* tmp;

	for(size_t i = index; i >= 0 && i < drawables[layer].size();){
		if(i > 0 && drawables[layer][i - 1]->renderOffset() > co){
			tmp = drawables[layer][i];
			drawables[layer][i] = drawables[layer][i - 1];
			drawables[layer][i - 1] = tmp;
			i--;
		}
		else if(i < drawables[layer].size() - 1 && drawables[layer][i + 1]->renderOffset() < co){
			tmp = drawables[layer][i];
			drawables[layer][i] = drawables[layer][i + 1];
			drawables[layer][i + 1] = tmp;
			i++;
		}
		else{
			break;
		}
	}
}

void World::addDrawable(drawable::Drawable* drawable, const Layer& layer){
	drawable->reference = drawable->animations[drawable->currentAnimation]->textures[0];
	if(!drawable->cb.shouldCollide){
		drawable->cb = manager->collisionManager->getCollisionBox(drawable->reference);
	}
	if(drawable->cb.shouldCollide){
		collidables.push_back(drawable);
	}
	drawable->calcRenderOffset();
	doSwap[layer] = true;
	insertDrawableAt(drawable, layer, binarySearchRenderOffset(drawable, layer));
	entities.push_back((Entity*) drawable);
}

void World::changeLayer(drawable::Drawable* drawable, const Layer& from, const Layer& to){
	drawables[from].erase(remove(drawables[from].begin(), drawables[from].end(), drawable));
	doSwap[to] = true;
	insertDrawableAt(drawable, to, binarySearchRenderOffset(drawable, to));
}

Target* World::drawableAt(const float& x, const float& y, const Layer& layer){
	float rwx = gi::wx(x);
	float rwy = gi::wy(y);
	float dist;
	size_t min = binarySearchRenderOffset(rwy - MAX_COLLISION_DISTANCE, layer);
	size_t max = binarySearchRenderOffset(rwy + MAX_COLLISION_DISTANCE, layer);

	if(max >= drawables[layer].size()){
		if(drawables[layer].size() == 0){
			max = 0;
		}
		else{
			max = drawables[layer].size() - 1;
		}
	}
	for(size_t i = max; i < drawables[layer].size() && i >= min; i--){
		drawable::Drawable* d = drawables[layer][i];

		if((dist = rwx - d->position.x + rwy - d->position.y) > MAX_COLLISION_DISTANCE || dist < 0){
			continue;
		}

		CoreSprite* cs = d->getSprite(time());
		if(
			d->position.x < rwx
			&& d->position.x + cs->w() * d->scale  > rwx
			&& d->position.y < rwy
			&& d->position.y + cs->h() * d->scale > rwy
			){
			return new Target(
				d,
				layer,
				(rwx - d->position.x) * gi::dx(),
				(rwy - d->position.y) * gi::dy()
				);
		}
	}
	return NULL;
}

unsigned int save_helper(Configuration& c, std::vector<drawable::Drawable*>& ds, std::string layer){
	for(size_t i = 0; i < ds.size(); i++){
		drawable::Drawable* d = ds[i];
		if(!d->isAlive()){
			continue;
		}
		std::string sub = "drawables." + layer + "." + std::to_string(i);
		if(d->currentAnimation != "default"){
			c.set(sub + ".currentAnimation", d->currentAnimation);
		}
		if(d->nextAnimation != d->currentAnimation){
			c.set(sub + ".nextAnimation", d->nextAnimation);
		}
		if(d->scale != 1.0f){
			c.set(sub + ".scale", d->scale);
		}
		if(d->health != 1.0f){
			c.set(sub + ".health", d->health);
		}
		c.set(sub + ".position", d->position.fv());
		if(d->velocity.length() > 0.0f){
			c.set(sub + ".velocity", d->velocity.fv());
		}
		if(d->hideUnderCamera){
			c.set(sub + ".hideUnderCamera", d->hideUnderCamera);
		}
		sub += ".animations";
		for(auto &ent : d->animations){
			drawable::Animation* a = d->animations[ent.first];
			std::string suba = sub + "." + ent.first;
			c.set(suba + ".textures", a->textures);
			if(a->timing.asMilliseconds() != 0){
				c.set(suba + ".timing", a->timing.asMilliseconds());
			}
		}
	}
	return ds.size();
}

void World::save(File& f){
	sf::Clock cl;
	Configuration c;

	if(backgroundName.length() > 0){
		c.set("background", backgroundName);
	}
	unsigned int total = 0;
	for(size_t l = 0; l < NUM_LAYERS; l++){
		Layer layer = Layer(l);
		total += save_helper(c, drawables[layer], layerToString(layer));
	}
	logger::timing(std::to_string(total) + " objects read in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	cl.restart();
	c.save(f);
	logger::timing("World configuration saved in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	logger::info("World saved: " + f.parent().name() + "\\" + f.name());
}

unsigned int load_helper(Configuration& c, World* w, std::string layer, Manager* m){
	std::vector<std::string> cs = c.children("drawables." + layer);
	size_t size = cs.size();
	Layer l = parseLayer(layer);
	for(size_t i = 0; i < size; i++){
		std::string sub = "drawables." + layer + "." + std::to_string(i);
		drawable::Drawable* d = new drawable::Drawable();
		d->currentAnimation = c.stringValue(sub + ".currentAnimation");
		if(d->currentAnimation == ""){
			d->currentAnimation = "default";
		}
		d->nextAnimation = c.stringValue(sub + ".nextAnimation");
		if(d->nextAnimation == ""){
			d->nextAnimation = d->currentAnimation;
		}
		d->scale = c.floatValue(sub + ".scale");
		if(d->scale == 0.0f){
			d->scale = 1.0f;
		}
		d->health = c.floatValue(sub + ".health");
		if(d->health == 0.0f){
			d->health = 1.0f;
		}
		d->position = Vector(c.floatVector(sub + ".position"));
		d->position.x = ceil(d->position.x);
		d->position.y = ceil(d->position.y);
		d->velocity = Vector(c.floatVector(sub + ".velocity"));
		d->hideUnderCamera = c.boolValue(sub + ".hideUnderCamera");
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
		w->addDrawable(d, l);
	}
	return size;
}

void World::load(File& f){
	cleanAll(true);
	firstTick = true;

	sf::Clock cl;
	Configuration* c = new Configuration();

	c->load(f);
	logger::timing("World configuration loaded in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	cl.restart();
	backgroundName = c->stringValue("background");
	unsigned int total = 0;
	for(size_t l = 0; l < NUM_LAYERS; l++){
		total += load_helper(*c, this, layerToString(Layer(l)), manager);
	}
	delete c;
	background = manager->spriteManager->getBackground(backgroundName);
	logger::timing(std::to_string(total) + " objects added in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
	logger::info("World loaded: " + f.parent().name() + "\\" + f.name());
}

void World::cleanAll(const bool& all){
	for(auto &ent : drawables){
		ent.second.erase(
			remove_if(ent.second.begin(), ent.second.end(),
				[all](drawable::Drawable* d){ return all || !d->isAlive(); }),
			ent.second.end());
	}

	for(size_t i = 0; i < entities.size(); i++){
		if(all || !entities[i]->isAlive()){
			manager->soundManager->clear(entities[i]);
			delete entities[i];
			entities.erase(entities.begin() + (i--));
		}
	}
}