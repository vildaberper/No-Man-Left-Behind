#include "GI.h"

namespace gi{
	sf::RenderWindow* renderWindow;
	unsigned int drawCalls;
	unsigned long frameCount;
	sf::Time begin;
	float cameraX;
	float cameraY;
	sf::Font menuFont;

	bool initalize(sf::RenderWindow*& rw){
		//renderWindow = rw = new sf::RenderWindow(sf::VideoMode(1920, 1080, 32), "SFML", sf::Style::Fullscreen);
		renderWindow = rw = new sf::RenderWindow(sf::VideoMode(1280, 720, 32), "NMLB");
		drawCalls = frameCount = 0;

		cameraX = TARGET_WIDTH / 2;
		cameraY = TARGET_HEIGHT / 2;

		return menuFont.loadFromFile(c::fontDir.child("Arial.ttf").path());
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
	void draw(drawable::Drawable* drawable, const sf::Time& time){
		drawCalls++;

		sf::Sprite* s = drawable->getSprite(time);

		if ((time - begin).asSeconds() > 1){
			//logger::info("FPS: " + std::to_string(frameCount / (time - begin).asSeconds()));
			begin = time;
			frameCount = 0;
		}
		if (drawable->highlight){
			sf::RectangleShape rs = sf::RectangleShape();
			rs.setPosition(s->getGlobalBounds().left, s->getGlobalBounds().top);
			rs.setSize(sf::Vector2f(s->getGlobalBounds().width, s->getGlobalBounds().height));
			rs.setFillColor(sf::Color(0, 0, 0, 0));
			rs.setOutlineColor(sf::Color(255, 255, 0, 255));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);
		}
		renderWindow->draw(*s);
	}

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h){
		sf::RectangleShape rs = sf::RectangleShape();
		rs.setPosition(x, y);
		rs.setSize(sf::Vector2f(w, h));
		if (item->toggle != NULL && !item->toggle->hidden){
			rs.setFillColor(sf::Color(55, 55, 0, 225));
		}
		else{
			rs.setFillColor(sf::Color(0, 0, 0, 225));
		}
		rs.setOutlineColor(sf::Color(255, 255, 0, 255));
		rs.setOutlineThickness(1);
		renderWindow->draw(rs);

		float tw = w;
		switch (item->type){
		case TEXTURE:
			sf::Sprite* s = item->sprite;
			if (s == NULL){
				break;
			}
			float scx = w / s->getGlobalBounds().width;
			float scy = h / s->getGlobalBounds().height;
			float sc = scx < scy ? scx : scy;
			s->scale(sc, sc);
			s->setPosition(x + w - s->getGlobalBounds().width, y);
			renderWindow->draw(*s);
			tw -= s->getGlobalBounds().width;
			break;
		}

		sf::Text title = sf::Text();
		title.setFont(menuFont);
		title.setColor(sf::Color(255, 255, 255, 255));
		title.setString(item->title);

		float scx = tw / (title.getGlobalBounds().width + 10);
		float scy = h / (title.getGlobalBounds().height + 10);
		float sc = scx < scy ? scx : scy;
		title.scale(sc, sc);
		title.setOrigin(0, float(title.getCharacterSize() / 2));
		title.setPosition(x + 5 * dx(), y + h / 2 - 5 * dy());
		renderWindow->draw(title);
	}


	void draw(Menu* menu, const sf::Time& time){
		float x = menu->position.x * dx();
		float y = menu->position.y * dy();
		float dx_ = menu->size.x * dx();
		float dy_ = menu->size.y * dy();

		switch (menu->type){
		case VERTICAL:
		{
						 dy_ /= menu->items.size();
						 for (size_t i = 0; i < menu->items.size(); i++){
							 draw(menu->items[i], time, x, y + dy_ * i, dx_, dy_);
						 }
						 break;
		}
		case HORIZONTAL:
		{
						   dx_ /= menu->items.size();
						   for (size_t i = 0; i < menu->items.size(); i++){
							   draw(menu->items[i], time, x + dx_ * i, y, dx_, dy_);
						   }
						   break;
		}
		}
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