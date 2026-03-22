#ifndef __RPS_GUI_HPP
#define __RPS_GUI_HPP

#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>

class GUI 
{
	sf::RenderWindow& window;

public:
	GUI(sf::RenderWindow& window);
	void process_events(const std::optional<sf::Event> event);
	void update(sf::Time dt);
	void display();
};

#endif