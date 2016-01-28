#pragma once

#include "Logger.h"
#include "SFMLI.h"
#include "File.h"

/*class SubSound{
public:
	SubSound();
	~SubSound();

private:

};
*/

class SoundManager{
public:
	SoundManager();
	~SoundManager();

	void initialize(sf::RenderWindow* window);

	void finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);
};