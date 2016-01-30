#include "GI.h"

namespace gi{
	sf::RenderWindow* renderWindow;
	unsigned int drawCalls;
	unsigned long frameCount;
	sf::Time begin;
	float cameraX;
	float cameraY;

	bool initalize(sf::RenderWindow*& rw){
		//renderWindow = rw = new sf::RenderWindow(sf::VideoMode(1920, 1080, 32), "SFML", sf::Style::Fullscreen);
		renderWindow = rw = new sf::RenderWindow(sf::VideoMode(1280, 720, 32), "NMLB");
		drawCalls = frameCount = 0;

		cameraX = TARGET_WIDTH / 2;
		cameraY = TARGET_HEIGHT / 2;

		return true;
	}

	bool finalize(){
		if (renderWindow->isOpen()){
			renderWindow->close();
		}
		delete renderWindow;

		return true;
	}

	bool startOfFrame(){
		frameCount++;
		renderWindow->clear();
		return renderWindow->isOpen();
	}

	float dx(){
		return renderWindow->getSize().x / TARGET_WIDTH;
	}
	float dy(){
		return renderWindow->getSize().y / TARGET_HEIGHT;
	}

	// Drawcall
	bool draw(drawable::Drawable* drawable, const sf::Time& time){
		drawCalls++;

		sf::Sprite* s = drawable->getSprite(time);

		if ((time - begin).asSeconds() > 1){
			logger::info("FPS: " + std::to_string(frameCount / (time - begin).asSeconds()));
			begin = time;
			frameCount = 0;
		}

		renderWindow->draw(*s);

		return true;
	}

	// endOfFrame
	bool endOfFrame(){
		renderWindow->display();
		if (frameCount % 60 == 0){
			// TODO: DrawCalls per 60 frame (average
		}

		return true;
	}
}