#pragma once

#include "SFMLI.h"
#include "Drawable.h"

namespace gi{
	static sf::RenderWindow* renderWindow;
	static unsigned int drawCalls;
	static unsigned long frameCount;

	// Initialize \ Finalize //
	static bool initalizeGI(sf::RenderWindow*& rw){
		rw = new sf::RenderWindow(sf::VideoMode(1280, 720), "NMLB");
		renderWindow = rw;
		drawCalls = 0;
		frameCount = 0;
	}
	static bool finalizeGI(){
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
	static bool drawSprite(Drawable* drawable){
		drawCalls++;
		renderWindow->draw(*drawable->getSprite());
		return true;
	}
	// endOfFrame //
	static bool endOfFrame(){
		renderWindow->display();
		if (frameCount % 60 == 0){
			// TODO: DrawCalls per 60 frame
		}
		return true;
	}
}