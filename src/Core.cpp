#include "Core.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

Core& Core::get_instance() 
{
	if (!instance) {
		instance = new Core();
	}
	return *instance;
}

Core::Core() 
	: 
	window_title{"rps_life"},
	window_rect({0, 0}, {800, 600}),
	window(sf::VideoMode(window_rect.size), window_title),
	user_interface(window)
{
	Entity::loadMedia();
	// this->window.setVerticalSyncEnabled(true);
	this->spawn_type = Entity::ROCK;
	this->delta_time = sf::Time::Zero;
}

void Core::run()
{
	int group_radius = 125;

	entity_group_system.spawn_group(Entity::SCISSORS, 20, {100, 300}, group_radius);
	entity_group_system.spawn_group(Entity::PAPER, 20, {350, 100}, group_radius);
	entity_group_system.spawn_group(Entity::ROCK, 20, {700, 400}, group_radius);

	while(this->window.isOpen())
	{
		this->delta_time = this->delta_clock.restart();
		while(const std::optional event = window.pollEvent())
			this->process_events(event);

		user_interface.update(this->delta_time);
		entity_group_system.update(this->delta_time);

		window.clear(sf::Color::White);

		window.draw(entity_group_system);

		user_interface.display();
		this->window.display();
	}
}

const sf::Rect<std::uint32_t>& Core::get_window_rect() 
{
	return this->window_rect;
}

GUI& Core::get_user_interface()
{
	return this->user_interface;
}

void Core::message_to_title(sf::String msg)
{
	this->window.setTitle(this->window_title + ": [" + msg + "]");
}

void Core::message_to_stdout(sf::String msg)
{
	std::cout << msg.toAnsiString() << std::endl;
}

void Core::message_to_all_output(sf::String msg)
{
	this->message_to_title(msg);
	this->message_to_stdout(msg);
};

void Core::process_events(const std::optional<sf::Event> event)
{
	user_interface.process_events(event);
	if (event->is<sf::Event::Closed>())
	{
		this->window.close();
	}
	else if (const auto* MouseKey = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if ((MouseKey->button == sf::Mouse::Button::Left) && !user_interface.is_want_capture_mouse()) 
		{
			entity_group_system.spawn_group(
				user_interface.get_entity_type(), 
				user_interface.quantity, 
				sf::Vector2f(MouseKey->position), 
				user_interface.spawn_area_radius
			);
		}
	}
}
