#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

const sf::FloatRect g_window_rect({0, 0}, {800, 600});

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(g_window_rect.size)), "rps_life");
    window.setVerticalSyncEnabled(true);
    
    Entity::loadMedia();
    
    EntityGroupSystem EGS(20, 20, 20, {100, 300}, {350, 100}, {700, 400}, 250);

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::White);

        EGS.update();
        window.draw(EGS);
        
        window.display();
    }
    
}