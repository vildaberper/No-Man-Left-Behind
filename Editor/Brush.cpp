#include "Brush.h"

Brush::Brush(){

}

Brush::~Brush(){

}

bool Brush::paint(const float& x, const float& y, const float& radius, const float& density, Manager*& manager, World*& world, const Layer& layer){
	if(objects.size() == 0){
		return false;
	}

	float angle = 2.0f * PI * random::random();
	float radius_ = radius * random::random();
	float x_ = x + cos(angle) * radius_;
	float y_ = y + sin(angle) * radius_;

	std::string sprite = objects[random::random(objects.size() - 1)];
	CoreSprite* s = manager->spriteManager->getSprite(sprite);
	drawable::Drawable* d = new drawable::Drawable();
	drawable::Animation* a = new drawable::Animation();

	d->scale = scaleLower + (scaleUpper - scaleLower) * random::random();
	a->textures.push_back(sprite);
	a->sprites.push_back(s);
	a->timing = sf::milliseconds(0);
	d->animations["default"] = a;
	d->currentAnimation = d->nextAnimation = "default";

	d->position.x = x_ - (d->cb.offset.x+ d->cb.size.x / 2.0f) * s->w() * d->scale;
	d->position.y = y_ - (d->cb.offset.y + d->cb.size.y / 2.0f) * s->h() * d->scale;

	if(layer == LAYER0){
		if(world->background != NULL){
			int w = world->background->getSize().x;
			int h = world->background->getSize().y;
			float x = d->position.x - float(w);
			float y = d->position.y - float(h);
			x = ceil(x / w) * w;
			y = ceil(y / h) * h;
			d->position.x = x;
			d->position.y = y;
		}
	}

	if(world->drawables[layer].size() > 0){
		sf::FloatRect fr = d->bounds(world->time());
		float cx = fr.left + fr.width / 2.0f;
		float cy = fr.top + fr.height / 2.0f;
		bool passed = true;

		d->calcRenderOffset();

		size_t min = world->binarySearchRenderOffset(d->renderOffset() - MAX_COLLISION_DISTANCE, layer);
		size_t max = std::min(world->binarySearchRenderOffset(d->renderOffset() + MAX_COLLISION_DISTANCE, layer), world->drawables[layer].size() - 1);

		for(size_t i = min; i <= max; i++){
			drawable::Drawable* d0 = world->drawables[layer][i];
			if(math::interv(cx, d0->position.x) + math::interv(cy, d0->position.y) > MAX_COLLISION_DISTANCE){
				continue;
			}
			sf::FloatRect fr0 = d0->bounds(world->time());

			if(fr.intersects(fr0) || (layer != LAYER0 && math::distance(cx, cy, fr0.left + fr0.width / 2.0f, fr0.top + fr0.height / 2.0f) < density)){
				passed = false;
				break;
			}
		}
		if(passed){
			world->addDrawable(d, layer);
		}
		else{
			delete d;
			delete a;
		}
		return passed;
	}
	world->addDrawable(d, layer);
	return true;
}

void Brush::erase(const float& x, const float& y, const float& radius, World*& world, const Layer& layer){
	if(world->drawables[layer].size() > 0){
		size_t min = world->binarySearchRenderOffset(y - radius - MAX_COLLISION_DISTANCE, layer);
		size_t max = std::min(world->binarySearchRenderOffset(y + radius + MAX_COLLISION_DISTANCE, layer), world->drawables[layer].size() - 1);

		for(size_t i = min; i <= max; i++){
			drawable::Drawable* d0 = world->drawables[layer][i];
			sf::FloatRect fr0 = d0->bounds(world->time());

			if(math::distance(x, y, fr0.left + fr0.width / 2.0f, fr0.top + fr0.height / 2.0f) < radius){
				d0->kill();
			}
		}
	}
}