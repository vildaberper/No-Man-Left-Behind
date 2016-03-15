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
	sf::Font textFont;

	bool showCursor = true;
	bool hasCursor = false;
	drawable::Drawable* cursor = NULL;

	bool defaultCursor = true;

	Vector relativeOffset;

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
		bool success = menuFont.loadFromFile(c::menuFont.path()) && textFont.loadFromFile(c::textFont.path());

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
		if(c::showLog){
			for(unsigned int i = 0; i < logger::history.size(); i++){
				draw(logger::history[logger::history.size() - i - 1], 10 * dxiz(), (TARGET_HEIGHT - 125 - 40 * i) * dyiz(), (TARGET_WIDTH - 20) * dxiz(), 40 * dyiz());
			}
		}
	}

	void draw(const MenuItem* item, const sf::Time& time, const float& x, const float& y, const float& w, const float& h, const bool& drawElementBackgrounds){
		if(drawElementBackgrounds){
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

			if(item->highlight || (item->toggle != NULL && !item->toggle->hidden)){
				int c = int(205 + 50 * sin(time.asMilliseconds() / 100.0f));
				s->setColor(sf::Color(c, c, c, 255));
			}
			else{
				s->setColor(sf::Color(255, 255, 255, 255));
			}

			renderWindow->draw(*s);
			tw -= s->getGlobalBounds().width;
			break;
		}

		sf::Text title = sf::Text();
		title.setFont(menuFont);
		title.setColor(sf::Color(255, 255, 255, 255));
		title.setString(item->title);

		if(item->alternativeText){
			float scx = w / 2.0f / (title.getGlobalBounds().width + 10);
			float scy = h / 2.0f / (title.getGlobalBounds().height + 10);
			float sc = scx < scy ? scx : scy;
			title.scale(sc, sc);
			title.setOrigin(0, float(title.getCharacterSize()));

			float xp = x + w / 2 + 5 * dxiz();
			float yp = y + h - 5 * dyiz();
			float r = sc * 1.5f;

			title.setColor(sf::Color(0, 0, 0, 255));
			float a = 0.0f;
			for(unsigned int i = 0; i < 8; i++){
				title.setPosition(xp + r * cos(a), yp + r * sin(a));
				renderWindow->draw(title);
				a += PI / 4;
			}
			title.setColor(sf::Color(255, 255, 255, 255));
			title.setPosition(xp, yp);
			renderWindow->draw(title);
		}
		else{
			float scx = tw / (title.getGlobalBounds().width + 10);
			float scy = h / (title.getGlobalBounds().height + 10);
			float sc = scx < scy ? scx : scy;
			title.scale(sc, sc);
			title.setOrigin(0, float(title.getCharacterSize() / 2));
			title.setPosition(x + 5 * dxiz(), y + h / 2 - 5 * dyiz());
			renderWindow->draw(title);
		}
	}

	void draw(Menu* menu, const sf::Time& time){
		float x = menu->position.x * dxiz();
		float y = menu->position.y * dyiz();
		float dx_ = menu->size.x * dxiz();
		float dy_ = menu->size.y * dyiz();

		if(menu->background != NULL && menu->background->isValid()){
			draw(menu->background, x, y, dx_, dy_);
		}

		x += menu->leftOffset * dxiz();
		y += menu->topOffset * dyiz();

		dx_ = (menu->size.x - menu->leftOffset - menu->rightOffset) * dxiz();
		dy_ = (menu->size.y - menu->topOffset - menu->bottomOffset) * dyiz();

		switch(menu->type){
		case VERTICAL:
		{
			dy_ /= menu->items.size();
			for(size_t i = 0; i < menu->items.size(); i++){
				draw(menu->items[i], time, x, y + dy_ * i, dx_, dy_, menu->drawElementBackgrounds);
			}
			break;
		}
		case HORIZONTAL:
		{
			dx_ /= menu->items.size();
			for(size_t i = 0; i < menu->items.size(); i++){
				draw(menu->items[i], time, x + dx_ * i, y, dx_, dy_, menu->drawElementBackgrounds);
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
			if(progressbar->customColors){
				rs.setFillColor(progressbar->bgColor);
				rs.setOutlineColor(progressbar->oColor);
			}
			else{
				rs.setFillColor(sf::Color(0, 0, 0, 185));
				rs.setOutlineColor(sf::Color(255, 255, 0, 255));
			}
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
			if(progressbar->customColors){
				rs.setFillColor(progressbar->pbColor);
			}
			else{
				rs.setFillColor(sf::Color(55, 55, 0, 255));
			}
			rs.setOutlineThickness(0);
			renderWindow->draw(rs);
		}
	}

	void draw(TexBar* texbar, const float& x, const float& y, const float& w, const float& h){
		if(texbar->left == NULL || texbar->right == NULL){
			sf::Sprite s = sf::Sprite(*texbar->middle);

			s.setPosition(x, y);
			s.scale(
				w / texbar->middle->getSize().x,
				h / texbar->middle->getSize().y
				);
			renderWindow->draw(s);
		}
		else{
			bool mrepeated = texbar->middle->isRepeated();
			texbar->middle->setRepeated(true);

			bool rrepeated = texbar->right->isRepeated();
			texbar->right->setRepeated(true);

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

			texbar->middle->setRepeated(mrepeated);
			texbar->right->setRepeated(rrepeated);
		}
	}

	void draw(const std::vector<std::string>& text, const float& x, const float& y, const float& w, const float& h, const sf::Font& font){
		float dh = std::min(20.0f, h / float(text.size()));
		float dhh = dh + 10.0f;
		for(size_t i = 0; i < text.size(); i++){
			sf::Text title = sf::Text();
			title.setFont(font);
			title.setColor(sf::Color(0, 0, 0, 255));
			title.setString(text[i]);
			title.setCharacterSize(30);
			float sc = std::min(0.6f, w / (title.getGlobalBounds().width + 10));
			title.scale(sc, sc);
			title.setOrigin(0, 0);
			title.setPosition(x + 5 * dxiz(), y + i * dhh * dyiz() - 5 * dyiz());
			renderWindow->draw(title);
		}

		if(collisionBoxes){
			sf::RectangleShape rs;
			rs.setPosition(x, y);
			rs.setSize(sf::Vector2f(w, h));
			rs.setFillColor(sf::Color(0, 0, 0, 0));
			rs.setOutlineColor(sf::Color(255, 0, 0, 255));
			rs.setOutlineThickness(1);
			renderWindow->draw(rs);
		}
	}

	void darken(const float& darkness){
		sf::RectangleShape rs;
		if(darkness > 1.0f){
			rs.setFillColor(sf::Color(0, 0, 0, 255));
		}
		else if(darkness <= 0.0f){
			return;
		}
		else{
			rs.setFillColor(sf::Color(0, 0, 0, int(255 * darkness)));
		}
		rs.setOutlineThickness(0.0f);
		rs.setPosition(0.0f, 0.0f);
		rs.setSize(sf::Vector2f(wx(), wy()));
		renderWindow->draw(rs);
	}

	void draw(CoreSprite& sprite, const float& x, const float& y, const float& w, const float& h, const float& a){
		if(a <= 0.0f){
			return;
		}

		sf::Sprite s = *sprite.sprite();

		s.setColor(sf::Color(255, 255, 255, a >= 1.0f ? 255 : sf::Uint8(a * 255.0f)));

		s.setPosition(x * dxiz(), y * dyiz());

		s.scale(
			(1.0f / s.getScale().x) *  (w * dxiz()) / sprite.w(),
			(1.0f / s.getScale().y) * (h * dyiz()) / sprite.h()
			);

		renderWindow->draw(s);
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
					cs->sprite()->getPosition().x - cursor->cb.offset.x * cs->w() * cursor->scale,
					cs->sprite()->getPosition().y - cursor->cb.offset.y * cs->h() * cursor->scale
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