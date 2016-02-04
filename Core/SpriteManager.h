#pragma once

#include "TextureManager.h"

class SpriteManager{
public:
	SpriteManager(){

	}
	~SpriteManager(){

	}

	void initialize(sf::RenderWindow* window){
		texMan = new TextureManager();
		texMan->initialize();
	}

	void finalize(sf::RenderWindow* window){
		texMan->finalize();
		delete texMan;
	}

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt){

	}

	sf::Sprite* getSprite(const std::string& catKey, const std::string& subKey){
		SubTexture localSubTex = *texMan->getTextureMap(catKey, subKey);
		int dx = int(ceil(localSubTex.texi->texture->getSize().x) / float(localSubTex.texi->width));
		int dy = int(ceil(localSubTex.texi->texture->getSize().y) / float(localSubTex.texi->height));
		if (
			localSubTex.x >= localSubTex.texi->width
			|| localSubTex.x < 0
			|| localSubTex.y >= localSubTex.texi->height
			|| localSubTex.y < 0
			){
			logger::warning("SubTexture " + catKey + "." + subKey + " is outside texture");

			// Returning the undefined a sprite with the undefine texture
			return new sf::Sprite(*texMan->getUndefinedTexture()->texi->texture);
		}

		return new sf::Sprite(
			*localSubTex.texi->texture,
			sf::IntRect(
			localSubTex.x * dx,
			localSubTex.y * dy,
			dx,
			dy
			)
			);
	}

	sf::Sprite* getSprite(const std::string& key){
		std::string::size_type index = key.find_first_of('.');

		return getSprite(key.substr(0, index), key.substr(index + 1));
	}

	sf::Texture* getBackground(const std::string& name){
		return texMan->getBackground(name);
	}

	const std::vector<std::string> backgrounds(){
		return texMan->backgrounds();
	}

	const std::vector<std::string> categories(){
		return texMan->categories();
	}
	const std::vector<std::string> members(const std::string& category){
		return texMan->members(category);
	}

	TextureManager* texMan;
};