#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Vec2f.hpp"
#include "Entity.hpp"
#include "EntityGroupSystem.hpp"

const sf::FloatRect g_window_rect(0,0,1280,720);

int main()
{   

    sf::RenderWindow win(sf::VideoMode(g_window_rect.width,g_window_rect.height), "rps_life");
    
    if( !ImGui::SFML::Init(win) )
    {
        std::cout << "ImGui error occured!\n";
    }
    
    win.setFramerateLimit(60);
    
    Entity::loadMedia();
    
    EntityGroupSystem EGS(20, 20, 20, {100, 300}, {350, 100}, {700, 400}, 250);
    bool fullscreen = false;

    sf::Clock deltaClock;
    sf::Event event;
    


    sf::Color bgColor(255,255,255);
    float color[3] = {static_cast<float>(bgColor.r) / 255.f, static_cast<float>(bgColor.g) / 255.f, static_cast<float>(bgColor.b) / 255.f};
    bool menuOpened = true;

    while(win.isOpen())
    {
        while(win.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(win, event);
            if (event.type == sf::Event::Closed)
                win.close();

            if(event.type == sf::Event::KeyPressed)
                switch (event.key.scancode)
                {
                case sf::Keyboard::Scan::F:
                    if(fullscreen)
                        win.create(sf::VideoMode(g_window_rect.width,g_window_rect.height), "rps_life");
                    else
                    win.create(sf::VideoMode(g_window_rect.width,g_window_rect.height), "rps_life", sf::Style::Fullscreen);
                    fullscreen = !fullscreen;
                    win.setFramerateLimit(60);
                    break;

                case sf::Keyboard::Scan::Q:
                    menuOpened = !menuOpened;
                    break;
                }
        }

        win.clear(bgColor);


        
        ImGui::SFML::Update(win, deltaClock.restart());
        
        if(menuOpened)
        {
            ImGui::Begin("Menu");
            ImGui::Text("Backgroud Color");
            if(ImGui::ColorEdit3("Change", color))
            {
                bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
                bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
                bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
            }

            ImGui::Text("Spawn Entity Group");
            
            ImGui::End();
        }

        EGS.update();
        win.draw(EGS);
        

        ImGui::SFML::Render(win);
        
        win.display();
    }
    ImGui::SFML::Shutdown();
}