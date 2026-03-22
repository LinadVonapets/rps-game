#include "Core.hpp"

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

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
	window(sf::VideoMode(window_rect.size), window_title)
{
	if(!ImGui::SFML::Init(this->window)) {
		std::string msg = "ImGui init failed! Exiting...";
		std::cerr << msg << std::endl;
		throw std::runtime_error(msg);
	}
	Entity::loadMedia();
	this->window.setVerticalSyncEnabled(true);
	this->spawn_type = Entity::ROCK;
	this->delta_time = sf::Time::Zero;
}

void Core::run()
{
	EntityGroupSystem EGS(20, 20, 20, {100, 300}, {350, 100}, {700, 400}, 250);

	while(this->window.isOpen())
	{
		this->delta_time = this->delta_clock.restart();
		while(const std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(this->window, *event);
			if (event->is<sf::Event::Closed>())
			{
				this->window.close();
			}
			else if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				switch(KeyPressed->scancode)
				{
					case sf::Keyboard::Scancode::R:
					{
						this->spawn_type = Entity::ROCK;
						message_to_all_output("Selected: Rock");
					}
					break;
					case sf::Keyboard::Scancode::P:
					{
						this->spawn_type = Entity::PAPER;
						message_to_all_output("Selected: Paper");
					}
					break;
					case sf::Keyboard::Scancode::S:
					{
						this->spawn_type = Entity::SCISSORS;
						message_to_all_output("Selected: Scissors");
					}
				}
			}
			else if (const auto* MouseKey = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (MouseKey->button == sf::Mouse::Button::Left) 
				{
					EGS.spawn_group(spawn_type, 1, sf::Vector2f(MouseKey->position), 0);
				}
			}
		}
		ImGui::SFML::Update(this->window, this->delta_time);

		ImGui::ShowDemoWindow();

		window.clear(sf::Color::White);

		EGS.update(this->delta_time);
		window.draw(EGS);

		ImGui::SFML::Render(this->window);
		this->window.display();
	}
}

const sf::Rect<std::uint32_t>& Core::get_window_rect() 
{
	return this->window_rect;
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