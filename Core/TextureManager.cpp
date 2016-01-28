#include "TextureManager.h"
#include "Constants.h"
#include "Logger.h"

// Constructor\Destructor
TextureManager::TextureManager(){
	
}

TextureManager::~TextureManager(){

}

// Initialize\Finalize
const bool TextureManager::initialize(){
	sf::Texture* t = new sf::Texture();
	File u = c::textureDir.child("undefined.png");
	if (!u.isFile() || !t->loadFromFile(u.path())){
		logger::warning("Undefined texture not found!");
		t->create(512, 512);
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

	// Load all them textures
	if (loadTextures()){
		logger::info("Textures loaded");
		return true;
	}
	else{
		logger::fatal("TexturesManager failed to initialize");
		return false;
	}
}

const bool TextureManager::finalize(){
	// Empty containers
	for (sf::Texture* t : textures){
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

// Load textures
bool TextureManager::loadTextures(){
	if (!c::textureDir.exists() || !c::textureDir.isDirectory()){
		return false;
	}
	for (File txtFile : c::textureDir.listFiles()){
		printf((txtFile.path() + "\n").data());
		if (!txtFile.isFile() || txtFile.extension() != "txt"){
			continue;
		}

		File pngFile = c::textureDir.child(txtFile.nameNoExtension() + ".png");

		sf::Texture* localTexture = new sf::Texture();
		if (!localTexture->loadFromFile(pngFile.name())){
			delete localTexture;
			continue;
		}

		Configuration config;
		if (!config.load(txtFile)){
			continue;
		}

		TexI* localTexi = new TexI();
		localTexi->texture = localTexture;
		int temp;
		localTexi->width = (temp = config.intVector("texture")[0]) == 0 ? 1 : temp;
		localTexi->height = (temp = config.intVector("texture")[1]) == 0 ? 1 : temp;
		textures.insert(localTexture);

		for (std::string texid : config.children("textures")){
			// Add subtexture (texid, x, y)
			SubTexture* sub = new SubTexture();
			sub->texi = localTexi;
			sub->x = config.intVector(texid)[0];
			sub->y = config.intVector(texid)[1];

			textureMap[pngFile.nameNoExtension()][texid] = sub;
		}
	}
	// All Texture loaded
	return true;
}