#pragma once

#include "Drawable.h"
#include "SFMLI.h"

namespace gi{
	static sf::RenderWindow* renderWindow;
	static unsigned int drawCalls;
	static unsigned long frameCount;

	// Initialize \ Finalize //
	static bool initalize(sf::RenderWindow*& rw){
		renderWindow = rw = new sf::RenderWindow(sf::VideoMode(1280, 720), "NMLB");
		drawCalls = frameCount = 0;

		return true;
	}

	static bool finalize(){
		if (renderWindow->isOpen()){
			renderWindow->close();
		}
		delete renderWindow;

		return true;
	}

	// StartOfFrame //
	static bool startOfFrame(){
		frameCount++;
		renderWindow->clear();

		return true;
	}

	// Drawcall //
	static bool draw(Drawable* drawable, const sf::Time& time){
		drawCalls++;
		renderWindow->draw(*drawable->getSprite(time));

		return true;
	}

	// endOfFrame //
	static bool endOfFrame(){
		renderWindow->display();
		if (frameCount % 60 == 0){
			// TODO: DrawCalls per 60 frame (average
		}

		return true;
	}
}