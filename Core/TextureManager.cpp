#include "TextureManager.h"
#include "Logger.h"
#include "Constants.h"
// Constructor\Destructor //
TextureManager::TextureManager(){
	sf::Texture* t = new sf::Texture();
	if (!t->loadFromFile("undefined.png")){
		logger::fatal("Undefined texture not found!");
	}
	textures.insert(t);
	// Load the undefined //
	undefined.x = 0;
	undefined.y = 0;
	undefined.texi = new TexI;
	undefined.texi->width = 1;
	undefined.texi->height = 1;
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
	for (sf::Texture* t : textures){
		delete t;
	}
	textures.clear();
	return true;
}
// Get a SubTexture, used to create sprites in the spritemanager //
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

		int w = config.intValue("image.w");
		int h = config.intValue("image.h");

		TexI* localTexi = new TexI;
		localTexi->texture = localTexture;
		localTexi->width = w;
		localTexi->height = h;

		textures.insert(localTexture);

		for (std::string texid : config.children("textures")){
			std::vector<int> pos = config.intVector(texid);
			int x = pos[0];
			int y = pos[1];

			// Add subtexture (texid, x, y)
			SubTexture sub;
			sub.texi = localTexi;
			sub.x = x;
			sub.y = y;

			textureMap[pngFile.nameNoExtension()][texid] = sub;
		}
	}
	// All Texture loaded
	return true;
}