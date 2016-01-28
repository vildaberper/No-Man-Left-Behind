#include "TextureManager.h"
#include "Logger.h"
#include "Constants.h"
// Constructor\Destructor //
TextureManager::TextureManager(){
	
}
TextureManager::~TextureManager(){

}
// Initialize\Finalize //
const bool TextureManager::textureManagerInitialize(){
	sf::Texture* t = new sf::Texture();
	if (!t->loadFromFile("undefined.png")){
		logger::fatal("Undefined texture not found!");
		return false;
	}
	textures.insert(t);
	// Load the undefined //
	undefined = new SubTexture();
	undefined->x = 0;
	undefined->y = 0;
	undefined->texi = new TexI();
	undefined->texi->width = 1;
	undefined->texi->height = 1;
	undefined->texi->texture = t;

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
	for (sf::Texture* t : textures){
		delete t;
	}
	textures.clear();
	return true;
}
// Functioner //
const TextureManager::SubTexture* TextureManager::getTextureMap(std::string& categoryKey, std::string& subKey){
	if (textureMap.count(categoryKey) == 0 || textureMap[categoryKey].count(subKey) == 0){
		logger::warning("SubTexture not found, key name: " + categoryKey + "." + subKey);
		return undefined;
	}
	else{
		return textureMap[categoryKey][subKey];
	}
}
const TextureManager::SubTexture* TextureManager::getUndefinedTexture(){
	return undefined;
}
// Load textures //
bool TextureManager::loadTextures(){
	for (File txtFile : c::textureDir.listFiles()){
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

		std::vector<int> dim = config.intVector("textures");

		TexI* localTexi = new TexI;
		localTexi->texture = localTexture;
		localTexi->width = dim[0];
		localTexi->height = dim[1];

		textures.insert(localTexture);

		for (std::string texid : config.children("textures")){
			std::vector<int> pos = config.intVector(texid);

			// Add subtexture (texid, x, y)
			SubTexture* sub = new SubTexture();
			sub->texi = localTexi;
			sub->x = pos[0];
			sub->y = pos[1];

			textureMap[pngFile.nameNoExtension()][texid] = sub;
		}
	}
	// All Texture loaded
	return true;
}