#pragma once

#include <map>

#include "SFMLI.h"

static const sf::Time DOUBLE_CLICK_TIME = sf::milliseconds(300);

class KeyboardEvent{
private:
	sf::Keyboard::Key key_;
	bool pressed_;
	bool first_;
	bool canceled_ = false;
public:
	KeyboardEvent(const sf::Keyboard::Key& key, const bool& pressed, const bool& first){
		key_ = key;
		pressed_ = pressed;
		first_ = first;
		canceled_ = false;
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

	bool isCancelled(){
		return canceled_;
	}

	void setCancelled(const bool& cancelled){
		canceled_ = cancelled;
	}
};
typedef void(*keyEventFunc)(KeyboardEvent&);

class MouseButtonEvent{
private:
	sf::Mouse::Button button_;
	bool pressed_;
	bool doubleClick_;
	int x_;
	int y_;
	bool canceled_ = false;
public:
	MouseButtonEvent(const sf::Mouse::Button& button, const bool& pressed, const bool& doubleClick, const int& x, const int& y){
		button_ = button;
		pressed_ = pressed;
		doubleClick_ = doubleClick;
		x_ = x;
		y_ = y;
		canceled_ = false;
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

	bool isCancelled(){
		return canceled_;
	}

	void setCancelled(const bool& cancelled){
		canceled_ = cancelled;
	}
};
typedef void(*mouseButtonEventFunc)(MouseButtonEvent&);

class MouseMoveEvent{
private:
	int x_;
	int y_;
	int dx_;
	int dy_;
	bool canceled_ = false;
public:
	MouseMoveEvent(const int& x, const int& y, const int& dx, const int& dy){
		x_ = x;
		y_ = y;
		dx_ = dx;
		dy_ = dy;
		canceled_ = false;
	}
	~MouseMoveEvent(){

	}

	int x(){
		return x_;
	}

	int y(){
		return y_;
	}

	int dx(){
		return dx_;
	}

	int dy(){
		return dy_;
	}

	bool isCancelled(){
		return canceled_;
	}

	void setCancelled(const bool& cancelled){
		canceled_ = cancelled;
	}
};
typedef void(*mouseMoveEventFunc)(MouseMoveEvent&);

class MouseWheelEvent{
private:
	int delta_;
	bool canceled_ = false;
public:
	MouseWheelEvent(const int& delta){
		delta_ = delta;
		canceled_ = false;
	}
	~MouseWheelEvent(){

	}

	int delta(){
		return delta_;
	}

	bool isCancelled(){
		return canceled_;
	}

	void setCancelled(const bool& cancelled){
		canceled_ = cancelled;
	}
};
typedef void(*mouseWheelEventFunc)(MouseWheelEvent&);

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

	void push(KeyboardEvent& event);
	void push(MouseButtonEvent& event);
	void push(MouseMoveEvent& event);
	void push(MouseWheelEvent& event);

	bool isPressed(const sf::Keyboard::Key& key);

	int mouseX();
	int mouseY();
private:
	bool keyStates[sf::Keyboard::KeyCount];
	bool firstPress[sf::Keyboard::KeyCount];

	int lastMoveX = -1;
	int lastMoveY = -1;

	int lastX = -1;
	int lastY = -1;
	sf::Time lastClick = sf::milliseconds(-DOUBLE_CLICK_TIME.asMilliseconds() - 1);

	unsigned long listenerId = 0;

	std::map<unsigned long, keyEventFunc> keyListeners;

	std::map<unsigned long, mouseButtonEventFunc> mouseButtonListeners;

	std::map<unsigned long, mouseMoveEventFunc> mouseMoveListeners;

	std::map<unsigned long, mouseWheelEventFunc> mouseWheelListeners;
};