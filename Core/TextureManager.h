#pragma once

#include <string>
#include <map>
#include <set>

#include "Logger.h"
#include "SFMLI.h"

// Help Classes
class TexI{
public:
	unsigned char width;	// Number of "sprites" in width
	unsigned char height;	// Number of "sprites" in height 
	sf::Texture* texture;
};

class SubTexture{
public:
	TexI* texi;
	unsigned char x;	// SubTexture width indexing
	unsigned char y;	// SubTexture hight indexing
};

class TextureManager{
public:
	// Constructor\Destructor
	TextureManager();
	~TextureManager();

	// Initialize\Finalize
	const bool initialize();
	const bool finalize();

	// Functioner
	const SubTexture* getTextureMap(const std::string& categoryKey, const std::string& subKey);
	const SubTexture* getUndefinedTexture();
private:
	// Texture Data //
	std::map<std::string, std::map<std::string, SubTexture*>> textureMap;
	std::set<sf::Texture*> textures;

	// Undefined sub for undefined textures
	SubTexture* undefined;

	// Texture
	bool loadTextures();
};