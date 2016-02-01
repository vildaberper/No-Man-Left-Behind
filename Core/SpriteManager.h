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
		float dx = float(localSubTex.texi->texture->getSize().x) / float(localSubTex.texi->width);
		float dy = float(localSubTex.texi->texture->getSize().y) / float(localSubTex.texi->height);

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
			int(round(localSubTex.x * dx)),
			int(round(localSubTex.y * dy)),
			int(round(dx)) - 1,
			int(round(dy)) - 1
			)
			);
	}

	sf::Sprite* getSprite(const std::string& key){
		std::string::size_type index = key.find_first_of('.');

		return getSprite(key.substr(0, index), key.substr(index + 1));
	}

	const std::vector<std::string> categories(){
		return texMan->categories();
	}
	const std::vector<std::string> members(const std::string& category){
		return texMan->members(category);
	}
private:
	TextureManager* texMan;
};