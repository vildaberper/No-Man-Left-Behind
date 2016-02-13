#pragma once

#include <string>
#include <map>
#include <set>

#include "Logger.h"
#include "SFMLI.h"
#include "File.h"

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
	bool hidden = false;
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

	sf::Texture* getBackground(const std::string& name);
	const std::vector<std::string> backgrounds();

	const std::vector<std::string> categories();
	const std::vector<std::string> members(const std::string& category);

	// Texture Data //
	std::map<std::string, std::map<std::string, SubTexture*>> textureMap;
	std::set<sf::Texture*> textures;
private:
	std::map<std::string, sf::Texture*> backgrounds_;

	// Undefined sub for undefined textures
	SubTexture* undefined;

	// Texture
	bool loadTextures();
	bool loadTexturesFromDir(File& dir);
};