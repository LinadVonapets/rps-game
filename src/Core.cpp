#include "Core.hpp"

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

Core& Core::get_instance() {
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
}

void Core::run() {
	EntityGroupSystem EGS(20, 20, 20, {100, 300}, {350, 100}, {700, 400}, 250);

	while(window.isOpen()) {
		while(const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear(sf::Color::White);

		EGS.update();
		window.draw(EGS);

		window.display();
	}
}