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

	enum class QuantityMode: int {
		PROPORTIONAL,
		LINEAR,
		MODE_LAST
	} quantity_mode;

	Entity::Type choice;

public:
	explicit GUI(sf::RenderWindow& window);
	void process_events(const std::optional<sf::Event> event);
	void update(sf::Time dt);
	void display();

	bool is_want_capture_mouse();

	Entity::Type get_entity_type();

	void show_tools_menu();

	float spawn_area_radius;
	sf::CircleShape spawn_area;

	int quantity;
	bool clear_all_entity;
};

#endif
