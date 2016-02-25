#pragma once

#include "GI.h"
#include "Menu.h"
#include "SFMLI.h"
#include "InputManager.h"

class MenuManager : InputListener{
public:
	MenuManager(InputManager* inputManager);
	~MenuManager();

	void initialize(sf::RenderWindow* window);

	void finalize(sf::RenderWindow* window);

	void tick(sf::RenderWindow* window, const sf::Time& time, const float& dt);

	void draw(const sf::Time& time);

	virtual void on(MouseButtonEvent& event);

	std::map<std::string, Menu*> menus;
private:
	InputManager* inputManager_;
};