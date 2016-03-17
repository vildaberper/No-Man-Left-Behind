#include "InputManager.h"

using namespace sf;
using namespace std;

InputManager::InputManager(){

}


InputManager::~InputManager(){

}

void InputManager::initialize(RenderWindow* window){
	for(unsigned int i = 0; i < sf::Keyboard::KeyCount; i++){
		keyStates[i] = false;
		firstPress[i] = false;
	}
}

void InputManager::finalize(RenderWindow* window){

}

void InputManager::tick(RenderWindow* window, const Time& time, const float& dt){
	Event event;

	for(unsigned int i = 0; i < sf::Keyboard::KeyCount; i++){
		if(keyStates[i]){
			firstPress[i] = false;
		}
	}

	while(window->pollEvent(event)){
		if(event.type == Event::Closed){
			window->close();
		}
		else if(event.type == sf::Event::KeyPressed){
			if(event.key.code == -1)
				return;
			if(keyStates[event.key.code]){
				firstPress[event.key.code] = false;
			}
			else{
				firstPress[event.key.code] = true;
			}
			keyStates[event.key.code] = true;
			push(KeyboardEvent(event.key.code, true, firstPress[event.key.code]));
		}
		else if(event.type == sf::Event::KeyReleased){
			if(event.key.code == -1)
				return;
			firstPress[event.key.code] = false;
			keyStates[event.key.code] = false;
			push(KeyboardEvent(event.key.code, false, firstPress[event.key.code]));
		}
		else if(event.type == sf::Event::MouseButtonPressed){
			bool doubleClick = time < lastClick + DOUBLE_CLICK_TIME && lastX == event.mouseButton.x && lastY == event.mouseButton.y;

			push(MouseButtonEvent(event.mouseButton.button, true, doubleClick, event.mouseButton.x, event.mouseButton.y));
			if(doubleClick){
				lastX = -1;
				lastY = -1;
				lastClick = milliseconds(-DOUBLE_CLICK_TIME.asMilliseconds() - 1);
			}
			else{
				lastX = event.mouseButton.x;
				lastY = event.mouseButton.y;
				lastClick = time;
			}
		}
		else if(event.type == sf::Event::MouseButtonReleased){
			push(MouseButtonEvent(event.mouseButton.button, false, false, event.mouseButton.x, event.mouseButton.y));
		}
		else if(event.type == sf::Event::MouseMoved){
			lastX = -1;
			lastY = -1;
			int dx = event.mouseMove.x - lastMoveX;
			int dy = event.mouseMove.y - lastMoveY;

			if(lastMoveX == -1 || lastMoveY == -1){
				dx = dy = 0;
			}
			push(MouseMoveEvent(lastMoveX = event.mouseMove.x, lastMoveY = event.mouseMove.y, dx, dy));
		}
		else if(event.type == sf::Event::MouseWheelScrolled){
			push(MouseWheelEvent(event.mouseWheelScroll.delta > 0.0f ? 1 : -1));
		}
		else if(event.type == sf::Event::Resized){
			window->setView(sf::View(sf::FloatRect(0.0f, 0.0f, float(event.size.width), float(event.size.height))));
		}
	}
}

unsigned long InputManager::registerListener(InputListener* listener){
	listenerId++;
	listeners[listenerId] = listener;

	return listenerId;
}
void InputManager::unregisterListener(const unsigned long& id){
	if(listeners.count(id) > 0){
		listeners.erase(id);
	}
}

void InputManager::push(KeyboardEvent& event){
	for(auto& elem : listeners){
		elem.second->on(event);
	}
}
void InputManager::push(MouseButtonEvent& event){
	for(auto& elem : listeners){
		elem.second->on(event);
	}
}
void InputManager::push(MouseMoveEvent& event){
	for(auto& elem : listeners){
		elem.second->on(event);
	}
}
void InputManager::push(MouseWheelEvent& event){
	for(auto& elem : listeners){
		elem.second->on(event);
	}
}

bool InputManager::isPressed(const sf::Keyboard::Key& key){
	return keyStates[key];
}
bool InputManager::isFirstPressed(const sf::Keyboard::Key& key){
	return firstPress[key];
}

int InputManager::mouseX(){
	return lastMoveX;
}
int InputManager::mouseY(){
	return lastMoveY;
}