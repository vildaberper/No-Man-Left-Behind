#include "GI.h"

namespace gi{
	sf::RenderWindow* renderWindow;
	unsigned int drawCalls = 0;
	unsigned long frameCount = 0;
	sf::Time begin;
	float cameraX;
	float cameraY;
	float cameraZ = 1.0f;

	bool smoothCamera = false;
	float cameraSmoothness = 5.0f;
	float cameraTargetX;
	float cameraTargetY;

	float WIDTH = TARGET_WIDTH;
	float HEIGHT = TARGET_HEIGHT;

	bool collisionBoxes = false;

	sf::Font menuFont;

	void zoom(const float& zoom){
		cameraZ = zoom > 0.01f ? zoom : 0.01f;
		WIDTH = TARGET_WIDTH / cameraZ;
		HEIGHT = TARGET_HEIGHT / cameraZ;
	}

	void camera(const float& dt){
		if(smoothCamera){
			Vector cv = Vector(cameraTargetX - cameraX, cameraTargetY - cameraY);
			if(cv.length() > 0.0f){
				cv *= dt * (cameraSmoothness - (cameraSmoothness / cv.length()));
				cameraX += cv.x;
				cameraY += cv.y;
			}
		}
		else{
			cameraX = cameraTargetX;
			cameraY = cameraTargetY;
		}
	}

	bool initalize(sf::RenderWindow*& rw){
		sf::VideoMode vm = sf::VideoMode(c::resX, c::resY, 32);

		if(c::fullscreen){
			if(c::autoResolution){
				vm = sf::VideoMode::getDesktopMode();
			}
			renderWindow = rw = new sf::RenderWindow(vm, c::WINDOW_TITLE, sf::Style::Fullscreen);
		}
		else{
			renderWindow = rw = new sf::RenderWindow(vm, c::WINDOW_TITLE);
		}
		renderWindow->setFramerateLimit(c::frameLimit);
		renderWindow->setVerticalSyncEnabled(c::verticalSync);

		drawCalls = frameCount = 0;

		cameraX = cameraTargetX = TARGET_WIDTH / 2;
		cameraY = cameraTargetY = TARGET_HEIGHT / 2;

		return menuFont.loadFromFile(c::fontDir.child("Arial.ttf").path());
	}

	bool finalize(){
		if(renderWindow->isOpen()){
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
		return renderWindow->getSize().x / WIDTH;
	}
	float dy(){
		return renderWindow->getSize().y / HEIGHT;
	}

	float dxiz(){
		return renderWindow->getSize().x / TARGET_WIDTH;
	}
	float dyiz(){
		return renderWindow->getSize().y / TARGET_HEIGHT;
	}

	// Drawcall
	void draw(sf::Sprite& sprite){
		renderWindow->draw(sprite);
	}

	void draw(drawable::Drawable* drawable, const sf::Time& time){
		drawCalls++;

		sf::Sprite* s = drawable->getSprite(time);

		if((time - begin).asSeconds() > 1){
			//logger::info("FPS: " + std::to_string(frameCount / (time - begin).asSeconds()));
			begin = time;
			frameCount = 0;
		}
		if(drawable->highlight){
			sf::RectangleShape rs = sf::RectangleShape();
			rs.setPosition(s->getGlobalBounds().left + 1, s->getGlobalBounds().top + 1);
			rs.setSize(sf::Vector2f(s->getGlobalBounds().width - 2, s->getGlobalBounds().height - 2));
			rs.setFillColor(sf::Color(0, 0, 0, 0));
			rs.setOutlineColor(sf::Color(255, 255, 0, 255));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);
		}

		draw(*s);

		if(collisionBoxes){
			sf::RectangleShape rs = sf::RectangleShape();
			rs.setPosition(s->getGlobalBounds().left + 1, s->getGlobalBounds().top + 1);
			rs.setSize(sf::Vector2f(s->getGlobalBounds().width - 2, s->getGlobalBounds().height - 2));
			rs.setFillColor(sf::Color(0, 0, 0, 0));
			rs.setOutlineColor(sf::Color(255, 255, 0, 155));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);

			rs.setPosition(s->getGlobalBounds().left, s->getGlobalBounds().top + s->getGlobalBounds().height * drawable->cb.renderOffset - 0.5f);
			rs.setSize(sf::Vector2f(s->getGlobalBounds().width, 1.0f));
			rs.setFillColor(sf::Color(0, 255, 0, 255));
			rs.setOutlineColor(sf::Color(0, 255, 0, 255));
			rs.setOutlineThickness(0);
			renderWindow->draw(rs);

			if(drawable->cb.shouldCollide){
				sf::FloatRect fr = drawable->bounds(time);
				rs.setPosition(
					round((fr.left - cameraX + WIDTH / 2) * dx()) + 0.375f,
					round((fr.top - cameraY + HEIGHT / 2) * dy()) + 0.375f
					);
				rs.setSize(sf::Vector2f(fr.width * gi::dx(), fr.height * gi::dy()));
				rs.setFillColor(sf::Color(0, 0, 0, 0));
				rs.setOutlineColor(sf::Color(255, 0, 0, 255));
				rs.setOutlineThickness(1);
				renderWindow->draw(rs);
			}
		}
	}

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h){
		sf::RectangleShape rs = sf::RectangleShape();
		rs.setPosition(x, y);
		rs.setSize(sf::Vector2f(w, h));
		if(item->toggle != NULL && !item->toggle->hidden){
			rs.setFillColor(sf::Color(55, 55, 0, 255));
		}
		else{
			rs.setFillColor(sf::Color(0, 0, 0, 195));
		}
		rs.setOutlineColor(sf::Color(255, 255, 0, 255));
		rs.setOutlineThickness(1);
		renderWindow->draw(rs);

		float tw = w;
		switch(item->type){
		case TEXTURE:
			sf::Sprite* s = item->sprite;
			if(s == NULL){
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
		title.setPosition(x + 5 * dxiz(), y + h / 2 - 5 * dyiz());
		renderWindow->draw(title);
	}


	void draw(Menu* menu, const sf::Time& time){
		float x = menu->position.x * dxiz();
		float y = menu->position.y * dyiz();
		float dx_ = menu->size.x * dxiz();
		float dy_ = menu->size.y * dyiz();

		switch(menu->type){
		case VERTICAL:
		{
			dy_ /= menu->items.size();
			for(size_t i = 0; i < menu->items.size(); i++){
				draw(menu->items[i], time, x, y + dy_ * i, dx_, dy_);
			}
			break;
		}
		case HORIZONTAL:
		{
			dx_ /= menu->items.size();
			for(size_t i = 0; i < menu->items.size(); i++){
				draw(menu->items[i], time, x + dx_ * i, y, dx_, dy_);
			}
			break;
		}
		}
	}

	void draw(ProgressBar* progressbar, const sf::Time& time){
		/*sf::RectangleShape rs = sf::RectangleShape();
		rs.setPosition(progressbar->position.x * dx(), progressbar->position.y* dy());
		rs.setSize(sf::Vector2f(progressbar->size.x, progressbar->size.y));
		rs.setFillColor(sf::Color(255, 0, 0, 255));
		rs.setOutlineColor(sf::Color(255, 0, 0, 255));
		rs.setOutlineThickness(1);
		gi::renderWindow->draw(rs);
		*/

		sf::Sprite* s = progressbar->bleft;
		s->setPosition(
			progressbar->position.x * dx(),
			progressbar->position.y * dy()
			);
		sf::FloatRect fl = s->getLocalBounds();
		s->scale(1.0f / s->getScale().x, 1.0f / s->getScale().y);
		float sc = (progressbar->size.y * dy()) / fl.height;
		s->scale(sc, sc);
		renderWindow->draw(*s);

		sf::RectangleShape rs = sf::RectangleShape();
		rs.setPosition(progressbar->position.x * dx(), progressbar->position.y * dy());
		rs.setSize(sf::Vector2f(progressbar->size.x * progressbar->progress * dx(), progressbar->size.y * dy()));
		rs.setFillColor(sf::Color(0, 255, 0, 100));
		rs.setOutlineColor(sf::Color(0, 0, 0, 0));
		rs.setOutlineThickness(0);
		renderWindow->draw(rs);
	}

	void darken(const float& darkness){
		sf::RectangleShape rs = sf::RectangleShape();
		rs.setFillColor(sf::Color(0, 0, 0, int(255 * darkness)));
		rs.setOutlineThickness(0);
		rs.setPosition(0, 0);
		rs.setSize(sf::Vector2f(float(renderWindow->getSize().x), float(renderWindow->getSize().y)));
		renderWindow->draw(rs);
	}

	// endOfFrame
	bool endOfFrame(){
		renderWindow->display();
		if(frameCount % 60 == 0){
			// TODO: DrawCalls per 60 frame (average
		}

		return true;
	}
}