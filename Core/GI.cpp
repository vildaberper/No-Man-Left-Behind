#include "GI.h"

namespace gi{
	sf::RenderWindow* renderWindow;
	float cameraX;
	float cameraY;
	float cameraZ;

	bool smoothCamera = false;
	float cameraSmoothness = 5.0f;
	float cameraTargetX;
	float cameraTargetY;

	bool logAlwaysActive = false;

	float WIDTH = TARGET_WIDTH;
	float HEIGHT = TARGET_HEIGHT;

	bool collisionBoxes = false;

	sf::Font menuFont;

	bool showCursor = true;
	bool hasCursor = false;
	drawable::Drawable* cursor = NULL;

	bool defaultCursor = true;

	sf::Clock clock;

	void zoom(const float& zoom){
		cameraZ = zoom > 0.01f ? zoom : 0.01f;
		WIDTH = TARGET_WIDTH / cameraZ;
		HEIGHT = TARGET_HEIGHT / cameraZ;
	}

	void camera(const float& dt){
		if(smoothCamera){
			Vector cv = Vector(cameraTargetX - cameraX, cameraTargetY - cameraY);
			float length = cv.length();
			if(length > 0.0f){
				cv *= std::min(1.0f, dt * (cameraSmoothness - (cameraSmoothness / length)));
				cameraX += cv.x;
				cameraY += cv.y;
			}
		}
		else{
			cameraX = cameraTargetX;
			cameraY = cameraTargetY;
		}
	}

	void resetCamera(){
		cameraZ = 1.0f;
		cameraX = cameraTargetX = TARGET_WIDTH / 2;
		cameraY = cameraTargetY = TARGET_HEIGHT / 2;
		WIDTH = TARGET_WIDTH / cameraZ;
		HEIGHT = TARGET_HEIGHT / cameraZ;
	}

	bool initalize(sf::RenderWindow*& rw){
		sf::Clock cl;

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

		resetCamera();
		bool success = menuFont.loadFromFile(c::menuFont.path());

		logger::timing("Graphics interface initialized in " + std::to_string(cl.getElapsedTime().asSeconds()) + " seconds");
		return success;
	}

	bool finalize(){
		if(renderWindow->isOpen()){
			renderWindow->close();
		}
		delete renderWindow;

		return true;
	}

	bool startOfFrame(){
		renderWindow->clear();
		return renderWindow->isOpen();
	}

	float wx(){
		return float(renderWindow->getSize().x);
	}
	float wy(){
		return float(renderWindow->getSize().y);
	}

	float dx(){
		return wx() / WIDTH;
	}
	float dy(){
		return wy() / HEIGHT;
	}

	float dxiz(){
		return wx() / TARGET_WIDTH;
	}
	float dyiz(){
		return wy() / TARGET_HEIGHT;
	}

	// +0.375f because bleeding. what
	float sx(float x){
		return round((x - gi::cameraX + gi::WIDTH / 2) * gi::dx()) + 0.375f;
	}
	float sy(float y){
		return round((y - gi::cameraY + gi::HEIGHT / 2) * gi::dy()) + 0.375f;
	}

	float wx(float x){
		return gi::cameraX - gi::WIDTH / 2 + x / gi::dx();
	}
	float wy(float y){
		return gi::cameraY - gi::HEIGHT / 2 + y / gi::dy();
	}

	// Drawcall
	void draw(sf::Sprite& sprite){
		renderWindow->draw(sprite);
	}

	void draw(CoreSprite& sprite){
		draw(*sprite.sprite());
	}

	void draw(drawable::Drawable* drawable, const sf::Time& time){
		if(
			!drawable->viewRelative
			&& (drawable->position.x < gi::cameraX - gi::WIDTH / 2 - 1000.0f
			|| drawable->position.x > gi::cameraX + gi::WIDTH / 2
			|| drawable->position.y < gi::cameraY - gi::HEIGHT / 2 - 1000.0f
			|| drawable->position.y > gi::cameraY + gi::HEIGHT / 2)
			){
			return;
		}

		CoreSprite* s = drawable->getSprite(time);

		draw(*s);

		if(collisionBoxes && !drawable->viewRelative){
			sf::RectangleShape rs;
			rs.setPosition(sx(drawable->position.x) + 1, sy(drawable->position.y) + 1);
			rs.setSize(sf::Vector2f(s->w() * drawable->scale * dx() - 2, s->h() * drawable->scale * dy() - 2));
			rs.setFillColor(sf::Color(0, 0, 0, 0));
			rs.setOutlineColor(sf::Color(255, 255, 0, 155));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);

			if(drawable->cb.shouldCollide){
				sf::FloatRect fr = drawable->bounds(time);
				rs.setPosition(
					sx(fr.left),
					sy(fr.top)
					);
				rs.setSize(sf::Vector2f(fr.width * gi::dx(), fr.height * gi::dy()));
				rs.setFillColor(sf::Color(0, 0, 0, 0));
				rs.setOutlineColor(sf::Color(255, 0, 0, 255));
				rs.setOutlineThickness(1);
				renderWindow->draw(rs);
			}

			rs.setPosition(sx(drawable->position.x), sy(drawable->renderOffset()) - 0.5f);
			rs.setSize(sf::Vector2f(s->w() * drawable->scale * dx(), 1.0f));
			rs.setFillColor(sf::Color(0, 255, 0, 255));
			rs.setOutlineColor(sf::Color(0, 255, 0, 255));
			rs.setOutlineThickness(0);
			renderWindow->draw(rs);
		}
	}

	void draw(const logger::LogEntry& logEntry, const float& x, const float& y, const float& w, const float& h){
		sf::Uint8 a = sf::Uint8(logAlwaysActive ? 185 : (185 * logEntry.fadeValue(sf::seconds(3.0f), sf::seconds(1.0f))));
		sf::Uint8 ta = sf::Uint8((float(a) / 185.0f) * 255.0f);
		sf::RectangleShape rs;

		rs.setPosition(x, y);

		rs.setFillColor(sf::Color(0, 0, 0, a));
		rs.setOutlineThickness(0);

		sf::Text title = sf::Text();
		title.setFont(menuFont);
		title.setColor(sf::Color(255, 255, 255, ta));
		title.setString(logEntry.message);

		float scx = w / (title.getGlobalBounds().width + 20);
		float scy = h / (title.getGlobalBounds().height + 10);
		float sc = scx < scy ? scx : scy;
		title.scale(sc, sc);
		title.setOrigin(0, float(title.getCharacterSize() / 2));
		title.setPosition(x + 10 * dxiz(), y + h / 2 - 5 * dyiz());

		rs.setSize(sf::Vector2f(title.getGlobalBounds().width + 20, h));
		renderWindow->draw(rs);

		renderWindow->draw(title);
	}

	void drawLog(){
		for(unsigned int i = 0; i < logger::history.size(); i++){
			draw(logger::history[logger::history.size() - i - 1], 10 * dxiz(), (TARGET_HEIGHT - 125 - 40 * i) * dyiz(), (TARGET_WIDTH - 20) * dxiz(), 40 * dyiz());
		}
	}

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h){
		if(item->background != NULL && item->background->isValid()){
			draw(item->background, x, y, w, h);
		}
		else{
			sf::RectangleShape rs;
			rs.setPosition(x, y);
			rs.setSize(sf::Vector2f(w, h));

			if(item->highlight || (item->toggle != NULL && !item->toggle->hidden)){
				rs.setFillColor(sf::Color(55, 55, 0, 255));
			}
			else{
				rs.setFillColor(sf::Color(0, 0, 0, 185));
			}
			rs.setOutlineColor(sf::Color(255, 255, 0, 255));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);
		}

		float tw = w;
		switch(item->type){
		case TEXTURE:
			if(item->sprite == NULL){
				break;
			}
			sf::Sprite* s = item->sprite->sprite();
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

		if(menu->background != NULL && menu->background->isValid()){
			draw(menu->background, x, y, dx_, dy_);
		}

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
		float x = progressbar->position.x * dxiz();
		float y = progressbar->position.y * dyiz();
		float w = progressbar->size.x * dxiz();
		float pw = w * progressbar->progress;
		float h = progressbar->size.y * dyiz();

		if(progressbar->background != NULL && progressbar->background->isValid()){
			draw(progressbar->background, x, y, w, h);
		}
		else{
			sf::RectangleShape rs;
			rs.setPosition(x, y);
			rs.setSize(sf::Vector2f(w, h));
			rs.setFillColor(sf::Color(0, 0, 0, 185));
			rs.setOutlineColor(sf::Color(255, 255, 0, 255));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);
		}

		if(progressbar->progressbar != NULL && progressbar->progressbar->isValid()){
			draw(progressbar->progressbar, x, y, pw, h);
		}
		else{
			sf::RectangleShape rs;
			rs.setPosition(x, y);
			rs.setSize(sf::Vector2f(pw, h));
			rs.setFillColor(sf::Color(55, 55, 0, 255));
			rs.setOutlineThickness(0);
			renderWindow->draw(rs);
		}
	}

	void draw(TexBar* texbar, const float& x, const float& y, const float& w, const float& h){
		float leftscale = h / texbar->left->getSize().y;
		float leftw = texbar->left->getSize().x * leftscale;
		int leftrw = int(ceil(leftw / leftscale));

		float rightscale = h / texbar->right->getSize().y;
		float rightw = texbar->right->getSize().x * rightscale;
		int rightrw = int(ceil(rightw / rightscale));
		
		float middlescale = h / texbar->middle->getSize().y;
		float middlew = w - leftw - rightw;
		int middlerw = int(ceil(middlew / middlescale));

		sf::Sprite s;

		if(leftw + rightw < w){
			s = sf::Sprite(*texbar->middle, sf::IntRect(0, 0, middlerw, int(ceil(h / middlescale))));
			s.setPosition(
				x + leftw,
				y
				);
			s.scale(middlescale, middlescale);
			renderWindow->draw(s);
		}
		else{
			float dw = w / 2;
			leftw = dw;
			leftrw = int(ceil(leftw / leftscale));

			rightw = dw;
			rightrw = int(ceil(rightw / rightscale));
		}

		s = sf::Sprite(*texbar->left, sf::IntRect(0, 0, leftrw, int(ceil(h / leftscale))));
		s.setPosition(
			x,
			y
			);
		s.scale(leftscale, leftscale);
		renderWindow->draw(s);

		s = sf::Sprite(*texbar->right, sf::IntRect(0, 0, -rightrw, int(ceil(h / rightscale))));
		s.setPosition(
			x + w,
			y
			);
		s.scale(-rightscale, rightscale);
		renderWindow->draw(s);
	}

	void darken(const float& darkness){
		sf::RectangleShape rs;
		rs.setFillColor(sf::Color(0, 0, 0, int(255 * darkness)));
		rs.setOutlineThickness(0.0f);
		rs.setPosition(0.0f, 0.0f);
		rs.setSize(sf::Vector2f(float(renderWindow->getSize().x), float(renderWindow->getSize().y)));
		renderWindow->draw(rs);
	}

	void background(CoreSprite& sprite){
		sf::Sprite s = *sprite.sprite();

		s.setPosition(0.0f, 0.0f);

		s.scale(
			(1.0f / s.getScale().x) *  wx() / sprite.w(),
			(1.0f / s.getScale().y) * wy() / sprite.h()
			);

		renderWindow->draw(s);
	}

	// endOfFrame
	bool endOfFrame(){
		if(showCursor){
			if(cursor != NULL){
				if(defaultCursor){
					renderWindow->setMouseCursorVisible(defaultCursor = false);
				}
				cursor->position = sf::Mouse::getPosition(*renderWindow);
				cursor->position.x /= dx();
				cursor->position.y /= dy();
				CoreSprite* cs = cursor->getSprite(clock.getElapsedTime());
				cs->sprite()->setPosition(
					cs->sprite()->getPosition().x - cursor->cb.offset.x * cs->w(),
					cs->sprite()->getPosition().y - cursor->cb.offset.y * cs->h()
					);
				draw(*cs->sprite());
			}
			else{
				renderWindow->setMouseCursorVisible(defaultCursor = true);
			}
		}
		else{
			if(defaultCursor){
				renderWindow->setMouseCursorVisible(defaultCursor = false);
			}
		}
		renderWindow->display();

		return true;
	}
}