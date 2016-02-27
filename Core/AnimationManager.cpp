#include "AnimationManager.h"

using namespace sf;
using namespace std;

AnimationManager::AnimationManager(){

}

AnimationManager::~AnimationManager(){

}

void AnimationManager::initialize(sf::RenderWindow* window, SpriteManager* sm){
	sf::Clock cl;

	undefined = new drawable::Animation();
	undefined->timing = milliseconds(0);
	undefined->textures.push_back("undefined");
	undefined->sprites.push_back(
		new CoreSprite(
			new sf::Sprite(
				*sm->texMan->getUndefinedTexture()->texi->texture
				),
			sm->texMan->getUndefinedTexture()->texi->texture->getSize().x,
			sm->texMan->getUndefinedTexture()->texi->texture->getSize().y
			)
		);

	loadFromDir(c::animationDir, sm);

	logger::timing("Animations initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
}

void AnimationManager::finalize(sf::RenderWindow* window){
	for(auto &ent : animations){
		delete ent.second;
	}
	animations.clear();
	delete undefined;
}

void AnimationManager::tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){

}

drawable::Animation* AnimationManager::getAnimation(const std::string& name){
	if(animations.count(name) == 0){
		return undefined->clone();
	}
	return animations[name]->clone();
}

bool AnimationManager::hasAnimation(const std::string& name){
	return animations.count(name) != 0;
}

void AnimationManager::loadFromDir(File& dir, SpriteManager* sm){
	if(!dir.exists() || !dir.isDirectory()){
		return;
	}
	for(File f : dir.listFiles()){
		if(f.isDirectory()){
			loadFromDir(f, sm);
		}
		if(!f.isFile() || f.extension() != "txt"){
			continue;
		}
		File pngFile = dir.child(f.nameNoExtension() + ".png");

		Texture* texture = new Texture();
		if(!texture->loadFromFile(pngFile.path())){
			delete texture;
			logger::warning("Failed to load animation: " + pngFile.path());
			continue;
		}
		sm->texMan->textures.insert(texture);

		Configuration config;
		if(!config.load(f)){
			logger::warning("Failed to load animation configuration: " + f.path());
			continue;
		}
		TexI* ti = new TexI();
		ti->texture = texture;
		ti->width = config.intVector("textures")[0];
		ti->height = config.intVector("textures")[1];

		drawable::Animation* a = new drawable::Animation();
		a->timing = sf::milliseconds(config.intValue("timing"));

		int frame = 0;
		int maxFrames = ti->width * ti->height - config.intValue("exclude");
		string category = dir.nameNoExtension() + "." + f.nameNoExtension();
		for(unsigned char y = 0; y < ti->height && frame < maxFrames; y++){
			for(unsigned char x = 0; x < ti->width && frame < maxFrames; x++){
				string name = to_string(frame++);
				SubTexture* st = new SubTexture();
				st->texi = ti;
				st->x = x;
				st->y = y;
				st->hidden = true;
				sm->texMan->textureMap[category][name] = st;
				a->sprites.push_back(sm->getSprite(category, name));
				a->textures.push_back(category + "." + name);
			}
		}
		animations[category] = a;
	}
}