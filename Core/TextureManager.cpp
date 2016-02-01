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
	Texture* t = new Texture();
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

const std::vector<std::string> TextureManager::categories(){
	std::vector<std::string> vec;

	for (auto ent : textureMap){
		vec.push_back(ent.first);
	}

	return vec;
}
const std::vector<std::string> TextureManager::members(const std::string& category){
	std::vector<std::string> vec;

	for (auto ent : textureMap[category]){
		vec.push_back(ent.first);
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
