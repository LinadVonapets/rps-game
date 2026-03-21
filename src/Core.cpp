#include "Core.hpp"

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

Core& Core::get_instance() 
{
	if (!instance) {
		instance = new Core();
	}
	return *instance;
}

Core::Core() 
	: 
	window_rect({0, 0}, {800, 600}),
	window(sf::VideoMode(window_rect.size), "rps_life")
{
	Entity::loadMedia();
	window.setVerticalSyncEnabled(true);
	spawn_type = Entity::ROCK;
}


void Core::run() 
{
	EntityGroupSystem EGS(20, 20, 20, {100, 300}, {350, 100}, {700, 400}, 250);

	while(window.isOpen()) {
		while(const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();

			} else if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>()) {
				switch(KeyPressed->scancode) {
					case sf::Keyboard::Scancode::R: {
						spawn_type = Entity::ROCK;
						std::cout << "Selected: \"Rock\"" << std::endl;
					}
					break;
					case sf::Keyboard::Scancode::P: {
						spawn_type = Entity::PAPER;
						std::cout << "Selected: \"Paper\"" << std::endl;
					}
					break;
					case sf::Keyboard::Scancode::S: {
						spawn_type = Entity::SCISSORS;
						
						std::cout << "Selected: \"Scissors\"" << std::endl;
					}
				}
			} else if (const auto* MouseKey = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (MouseKey->button == sf::Mouse::Button::Left) {
					EGS.spawn_group(spawn_type, 1, sf::Vector2f(MouseKey->position), 0);
				}
			}
		}

		window.clear(sf::Color::White);

		EGS.update();
		window.draw(EGS);

		window.display();
	}
}