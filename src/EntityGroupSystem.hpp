#ifndef __RPS_ENTITY_GROUP_SYSTEM_HPP
#define __RPS_ENTITY_GROUP_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <random>

#include "Entity.hpp"

class EntityGroupSystem: public sf::Drawable
{
private:
    std::vector<Entity> m_entities;
    
    //random constants
    std::random_device m_dev;
    std::mt19937 m_random_engine;

public:
    EntityGroupSystem(int amount_rocks, int amount_papers, int amount_scisissors, 
                      sf::Vector2f rocks_pos, sf::Vector2f papers_pos, sf::Vector2f scissors_pos, 
                      float group_radius);
    void spawn_group(Entity::Type type, int amount, sf::Vector2f pos, float radius);
    sf::Vector2f randomize_coord(float x, float y, float R_max);
    double get_random(double begin, double end);
    void update();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif