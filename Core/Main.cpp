#include <SFML/Graphics.hpp>

using namespace sf;

int main(){
	RenderWindow window(VideoMode(500, 500), "SFML works!");
	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);
	shape.setOrigin(100, 100);

	float dx = 0.0f;
	float dy = 0.0f;

	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();

		dx += 0.0001f;
		dy += 0.0001f;
		shape.setPosition(250, 250);
		shape.rotate(0.01f);
		shape.setFillColor(Color(int(255 * abs(sin(dx))), int(255 * abs(cos(dy / 2))), int(255 * abs(cos(dy))), int(55 + 200.0f * abs(cos(dy)))));
		shape.setScale(abs(sin(dx)), abs(cos(dy)));
		window.draw(shape);
		window.display();
	}

	return 0;
}