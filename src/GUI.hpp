#ifndef __RPS_GUI_HPP
#define __RPS_GUI_HPP

#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Entity.hpp"

class GUI 
{
	sf::RenderWindow& window;
	bool show;

	enum class Mode : int {
		MOVE_OR_DRAG,
		SPAWN,
		MODE_LAST
	} mode;

	Entity::Type choice;

public:
	GUI(sf::RenderWindow& window);
	void process_events(const std::optional<sf::Event> event);
	void update(sf::Time dt);
	void display();

	bool is_want_capture_mouse(); 

	Entity::Type get_entity_type();

	float spawn_area_radius;
	sf::CircleShape spawn_area;
};

#endif
