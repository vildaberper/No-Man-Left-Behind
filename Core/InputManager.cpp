#include "InputManager.h"

using namespace sf;
using namespace std;

InputManager::InputManager(){

}


InputManager::~InputManager(){

}

void InputManager::initialize(RenderWindow* window){
	for (unsigned int i = 0; i < sf::Keyboard::KeyCount; i++){
		keyStates[i] = false;
		firstPress[i] = false;
	}
}

void InputManager::finalize(RenderWindow* window){

}

void InputManager::tick(RenderWindow* window, const Time& time, const float& dt){
	Event event;

	while (window->pollEvent(event)){
		if (event.type == Event::Closed){
			window->close();
		}

		if (event.type == sf::Event::KeyPressed){
			if (event.key.code == -1)
				return;
			if (keyStates[event.key.code]){
				firstPress[event.key.code] = false;
			}
			else{
				firstPress[event.key.code] = true;
			}
			keyStates[event.key.code] = true;
			push(KeyboardEvent(event.key.code, true, firstPress[event.key.code]));
		}
		else if (event.type == sf::Event::KeyReleased){
			if (event.key.code == -1)
				return;
			keyStates[event.key.code] = false;
			push(KeyboardEvent(event.key.code, false, firstPress[event.key.code]));
		}
		else if (event.type == sf::Event::MouseButtonPressed){
			bool doubleClick = time < lastClick + DOUBLE_CLICK_TIME && lastX == event.mouseButton.x && lastY == event.mouseButton.y;

			push(MouseButtonEvent(event.mouseButton.button, true, doubleClick, event.mouseButton.x, event.mouseButton.y));
			if (doubleClick){
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
		else if (event.type == sf::Event::MouseButtonReleased){
			push(MouseButtonEvent(event.mouseButton.button, false, false, event.mouseButton.x, event.mouseButton.y));
		}
		else if (event.type == sf::Event::MouseMoved){
			lastX = -1;
			lastY = -1;
			int dx = event.mouseMove.x - lastMoveX;
			int dy = event.mouseMove.y - lastMoveY;

			if (lastMoveX == -1 || lastMoveY == -1){
				dx = dy = 0;
			}
			push(MouseMoveEvent(lastMoveX = event.mouseMove.x, lastMoveY = event.mouseMove.y, dx, dy));
		}
		else if (event.type == sf::Event::MouseWheelScrolled){
			push(MouseWheelEvent(event.mouseWheelScroll.delta > 0.0f ? 1 : -1));
		}
		else if (event.type == sf::Event::Resized){
			window->setView(sf::View(sf::FloatRect(0.0f, 0.0f, float(event.size.width), float(event.size.height))));
		}
	}
}

unsigned long InputManager::registerListener(const keyEventFunc& listener){
	listenerId++;
	keyListeners[listenerId] = listener;

	return listenerId;
}
unsigned long InputManager::registerListener(const mouseButtonEventFunc& listener){
	listenerId++;
	mouseButtonListeners[listenerId] = listener;

	return listenerId;
}
unsigned long InputManager::registerListener(const mouseMoveEventFunc& listener){
	listenerId++;
	mouseMoveListeners[listenerId] = listener;

	return listenerId;
}
unsigned long InputManager::registerListener(const mouseWheelEventFunc& listener){
	listenerId++;
	mouseWheelListeners[listenerId] = listener;

	return listenerId;
}

void InputManager::unregisterListener(const unsigned long& id){
	if (keyListeners.count(id) > 0){
		keyListeners.erase(id);
	}
	else if (mouseButtonListeners.count(id) > 0){
		mouseButtonListeners.erase(id);
	}
	else if (mouseMoveListeners.count(id) > 0){
		mouseMoveListeners.erase(id);
	}
	else if (mouseWheelListeners.count(id) > 0){
		mouseWheelListeners.erase(id);
	}
}

void InputManager::push(KeyboardEvent& event){
	std::map<unsigned long, keyEventFunc> keyListeners = InputManager::keyListeners;
	for (const auto& elem : keyListeners){
		elem.second(event);
	}
}
void InputManager::push(MouseButtonEvent& event){
	std::map<unsigned long, mouseButtonEventFunc> mouseButtonListeners = InputManager::mouseButtonListeners;
	for (const auto& elem : mouseButtonListeners){
		elem.second(event);
	}
}
void InputManager::push(MouseMoveEvent& event){
	std::map<unsigned long, mouseMoveEventFunc> mouseMoveListeners = InputManager::mouseMoveListeners;
	for (const auto& elem : mouseMoveListeners){
		elem.second(event);
	}
}
void InputManager::push(MouseWheelEvent& event){
	std::map<unsigned long, mouseWheelEventFunc> mouseWheelListeners = InputManager::mouseWheelListeners;
	for (const auto& elem : mouseWheelListeners){
		elem.second(event);
	}
}

bool InputManager::isPressed(const sf::Keyboard::Key& key){
	return keyStates[key];
}

int InputManager::mouseX(){
	return lastMoveX;
}
int InputManager::mouseY(){
	return lastMoveY;
}