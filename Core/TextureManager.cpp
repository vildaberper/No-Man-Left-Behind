#include "TextureManager.h"
#include "File.h"
#include "Logger.h"
// Constructor\Destructor //
TextureManager::TextureManager(){
	sf::Texture* t = new sf::Texture();
	if (!t->loadFromFile("undefined.png")){
		std::printf("undefined.png failed to load");
	}
	textures.insert(t);
	// Load the undefined //
	undefined.x = 1;
	undefined.y = 3;
	undefined.texi = new TexI;
	undefined.texi->width = 2;
	undefined.texi->height = 4;
	undefined.texi->texture= t;
}
TextureManager::~TextureManager(){
	// Finalize
	textureManagerFinalize();
}
// Initialize\Finalize //
const bool TextureManager::textureManagerInitialize(){
	// Load all them textures
	if (loadTextures()){
		std::cout << "Textures loaded" << std::endl;
		return true;
	}
	else{
		logger::fatal("TexturesManager failed to initialize");
		return false;
	}
}
const bool TextureManager::textureManagerFinalize(){
	// Empty containers //
	
	return true;
}
// Functioner //
const TextureManager::SubTexture* TextureManager::getTextureMap(std::string& categoryKey, std::string& subKey){
	if (textureMap.count(categoryKey) == 0 || textureMap[categoryKey].count(subKey) == 0){
		logger::warning("SubTexture not found, key name: " + categoryKey + "." + subKey);
		return &undefined;
	}
	else{
		return &textureMap[categoryKey][subKey];
	}
}
const TextureManager::SubTexture* TextureManager::getUndefinedTexture(){
	return &undefined;
}
// Load textures //
bool TextureManager::loadTextures(){
	//for (File f; ){
	//	if (){

	//	}
	//	File png = 
	//	File txt;
	//}
	return true;
}