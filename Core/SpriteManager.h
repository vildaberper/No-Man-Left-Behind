#pragma once

#include "TextureManager.h"
TextureManager* texMan;

namespace sm{
	const static bool initializeSpriteManager(){
		texMan = new TextureManager();

		if (texMan->textureManagerInitialize()){
			return true;
		}
		else{
			logger::fatal("SpriteManager failed to initialze");
			return false;
		}
	}
	const static bool finializeSpriteManager(){
		if (texMan->textureManagerFinalize()){
			delete texMan;
			return true;
		}
		else{
			logger::fatal("SpriteManager failed to finalize");
			return false;
		}
	}
	void tick(sf::RenderWindow* window, clock_t, float dt){

	}

	static sf::Sprite* getSprite(std::string& catKey, std::string& subKey){
		TextureManager::SubTexture localSubTex = *texMan->getTextureMap(catKey, subKey);
		float dx = localSubTex.texi->texture->getSize().x / localSubTex.texi->width;
		float dy = localSubTex.texi->texture->getSize().y / localSubTex.texi->height;
		if (dx >= localSubTex.texi->width || dx < 0 || dy >= localSubTex.texi->height || dy < 0){
			logger::warning("SubTexture "+catKey+"."+subKey+" is outside texture");
			//  Returning the undefined a sprite with the undefine texture
			return new sf::Sprite(*texMan->getUndefinedTexture()->texi->texture);
		}
		return new sf::Sprite(*localSubTex.texi->texture, sf::IntRect(localSubTex.x * dx, localSubTex.y * dy, dx, dy));
	}
}