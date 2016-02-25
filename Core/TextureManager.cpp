#include "TextureManager.h"
#include "Constants.h"
#include "Logger.h"

using namespace std;
using namespace sf;

// Constructor\Destructor
TextureManager::TextureManager(){

}

TextureManager::~TextureManager(){

}

// Initialize\Finalize
const bool TextureManager::initialize(){
	sf::Clock cl;

	Texture* t = new Texture();
	File u = c::textureDir.child("undefined.png");
	if (!u.isFile() || !t->loadFromFile(u.path())){
		logger::warning("Undefined texture not found!");
		t->create(128, 128);
	}
	textures.insert(t);
	// Load the undefined
	undefined = new SubTexture();
	undefined->x = 0;
	undefined->y = 0;
	undefined->texi = new TexI();
	undefined->texi->width = 1;
	undefined->texi->height = 1;
	undefined->texi->texture = t;

	// Load backgrounds
	for (File pngFile : c::backgroundDir.listFiles()){
		Texture* t = new Texture();
		if (!t->loadFromFile(pngFile.path())){
			delete t;
			continue;
		}
		t->setRepeated(true);
		backgrounds_[pngFile.nameNoExtension()] = t;
	}

	// Load all them textures
	if (loadTextures()){
		logger::timing("Textures initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return true;
	}
	else{
		logger::fatal("Textures failed to initialize");
		return false;
	}
}

const bool TextureManager::finalize(){
	// Empty containers
	for (Texture* t : textures){
		delete t;
	}
	textures.clear();
	// TODO Delete subtextures
	return true;
}

// Get a SubTexture, used to create sprites in the spritemanager
const SubTexture* TextureManager::getTextureMap(const std::string& categoryKey, const  std::string& subKey){
	if (textureMap.count(categoryKey) == 0 || textureMap[categoryKey].count(subKey) == 0){
		logger::warning("SubTexture not found, key name: " + categoryKey + "." + subKey);
		return getUndefinedTexture();
	}
	else{
		return textureMap[categoryKey][subKey];
	}
}

const SubTexture* TextureManager::getUndefinedTexture(){
	return undefined;
}

Texture* TextureManager::getBackground(const std::string& name){
	if (backgrounds_.count(name) == 0){
		return NULL;
	}
	return backgrounds_[name];
}

const std::vector<std::string> TextureManager::backgrounds(){
	std::vector<std::string> vec;

	for (auto ent : backgrounds_){
		vec.push_back(ent.first);
	}

	return vec;
}

const std::vector<std::string> TextureManager::categories(){
	std::vector<std::string> vec;

	for (auto &ent : textureMap){
		bool hidden = true;
		for (auto &sub : ent.second){
			if (!sub.second->hidden){
				hidden = false;
				break;
			}
		}
		if (!hidden){
			vec.push_back(ent.first);
		}
	}

	return vec;
}
const std::vector<std::string> TextureManager::members(const std::string& category){
	std::vector<std::string> vec;

	for (auto &ent : textureMap[category]){
		if (!ent.second->hidden){
			vec.push_back(ent.first);
		}
	}

	return vec;
}

// Load textures
bool TextureManager::loadTextures(){
	return loadTexturesFromDir(c::textureDir);
}

bool TextureManager::loadTexturesFromDir(File& dir){
	if (!dir.exists() || !dir.isDirectory()){
		return false;
	}
	bool success = true;
	for (File txtFile : dir.listFiles()){
		if (txtFile.isDirectory()){
			success = !loadTexturesFromDir(txtFile) ? false : success;
		}
		if (!txtFile.isFile() || txtFile.extension() != "txt"){
			continue;
		}

		File pngFile = dir.child(txtFile.nameNoExtension() + ".png");

		Texture* localTexture = new Texture();
		if (!localTexture->loadFromFile(pngFile.path())){
			delete localTexture;
			logger::warning("Failed to load texture: " + pngFile.path());
			success = false;
			continue;
		}

		Configuration config;
		if (!config.load(txtFile)){
			logger::warning("Failed to load texture configuration: " + txtFile.path());
			success = false;
			continue;
		}

		TexI* localTexi = new TexI();
		localTexture->setSmooth(false);
		localTexi->texture = localTexture;
		int temp;
		localTexi->width = (temp = config.intVector("textures")[0]) == 0 ? 1 : temp;
		localTexi->height = (temp = config.intVector("textures")[1]) == 0 ? 1 : temp;
		textures.insert(localTexture);

		for (std::string texid : config.children("textures", false)){
			// Add subtexture (texid, x, y)
			SubTexture* sub = new SubTexture();
			sub->texi = localTexi;
			sub->x = config.intVector("textures." + texid)[0];
			sub->y = config.intVector("textures." + texid)[1];

			textureMap[pngFile.nameNoExtension()][texid] = sub;
		}
	}
	// All Texture loaded
	return success;
}