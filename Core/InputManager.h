#pragma once

#include <map>

#include "SFMLI.h"

static const sf::Time DOUBLE_CLICK_TIME = sf::milliseconds(300);

class KeyboardEvent{
private:
	sf::Keyboard::Key key_;
	bool pressed_;
	bool first_;
public:
	KeyboardEvent(const sf::Keyboard::Key& key, const bool& pressed, const bool& first){
		key_ = key;
		pressed_ = pressed;
		first_ = first;
	}
	~KeyboardEvent(){

	}

	sf::Keyboard::Key key(){
		return key_;
	}

	bool pressed(){
		return pressed_;
	}

	bool first(){
		return first_;
	}
};
typedef void(*keyEventFunc)(KeyboardEvent);

class MouseButtonEvent{
private:
	sf::Mouse::Button button_;
	bool pressed_;
	bool doubleClick_;
	int x_;
	int y_;
public:
	MouseButtonEvent(const sf::Mouse::Button& button, const bool& pressed, const bool& doubleClick, const int& x, const int& y){
		button_ = button;
		pressed_ = pressed;
		doubleClick_ = doubleClick;
		x_ = x;
		y_ = y;
	}
	~MouseButtonEvent(){

	}

	sf::Mouse::Button button(){
		return button_;
	}

	bool pressed(){
		return pressed_;
	}

	bool doubleClick(){
		return doubleClick_;
	}

	int x(){
		return x_;
	}

	int y(){
		return y_;
	}
};
typedef void(*mouseButtonEventFunc)(MouseButtonEvent);

class MouseMoveEvent{
private:
	int x_;
	int y_;
public:
	MouseMoveEvent(const int& x, const int& y){
		x_ = x;
		y_ = y;
	}
	~MouseMoveEvent(){

	}

	int x(){
		return x_;
	}

	int y(){
		return y_;
	}
};
typedef void(*mouseMoveEventFunc)(MouseMoveEvent);

class MouseWheelEvent{
private:
	int delta_;
public:
	MouseWheelEvent(const int& delta){
		delta_ = delta;
	}
	~MouseWheelEvent(){

	}

	int delta(){
		return delta_;
	}
};
typedef void(*mouseWheelEventFunc)(MouseWheelEvent);

class InputManager{
public:
	InputManager();
	~InputManager();

	void initialize(sf::RenderWindow* window);

	void finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	unsigned long registerListener(const keyEventFunc& listener);
	unsigned long registerListener(const mouseButtonEventFunc& listener);
	unsigned long registerListener(const mouseMoveEventFunc& listener);
	unsigned long registerListener(const mouseWheelEventFunc& listener);

	void unregisterListener(const unsigned long& id);

	void push(const KeyboardEvent& event);
	void push(const MouseButtonEvent& event);
	void push(const MouseMoveEvent& event);
	void push(const MouseWheelEvent& event);
private:
	bool keyStates[sf::Keyboard::KeyCount];
	bool firstPress[sf::Keyboard::KeyCount];

	int lastX = -1;
	int lastY = -1;
	sf::Time lastClick = sf::milliseconds(-DOUBLE_CLICK_TIME.asMilliseconds() - 1);

	unsigned long listenerId = 0;

	std::map<unsigned long, keyEventFunc> keyListeners;

	std::map<unsigned long, mouseButtonEventFunc> mouseButtonListeners;

	std::map<unsigned long, mouseMoveEventFunc> mouseMoveListeners;

	std::map<unsigned long, mouseWheelEventFunc> mouseWheelListeners;
};