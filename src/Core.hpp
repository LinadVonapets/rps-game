#ifndef __RPS_CORE_HPP
#define __RPS_CORE_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"
#include "GUI.hpp"

class Core 
{
private:
	static inline Core* instance = nullptr;
	
	// Do not change order of these fields, because we initialize them 
	// in member initializer list exactly in the order they are declared here
	sf::String window_title;
	sf::Rect<std::uint32_t> window_rect;
	sf::RenderWindow window;
	GUI user_interface;
	//////////////////////////////////////////////////////////////////

	EntityGroupSystem entity_group_system;

	Entity::Type spawn_type;

	sf::Clock delta_clock;
	sf::Time delta_time;

public:
	static Core& get_instance();

	Core();
	virtual ~Core() = default;

	void run();
	const sf::Rect<std::uint32_t>& get_window_rect();

	void message_to_title(sf::String msg);
	void message_to_stdout(sf::String msg);
	void message_to_all_output(sf::String msg);

private:
	Core(const Core&) = delete;
	Core& operator=(const Core&) = delete;
};

#endif
