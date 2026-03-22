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
	EntityGroupSystem();
	void spawn_group(Entity::Type type, int amount, sf::Vector2f pos, float radius);
	sf::Vector2f randomize_coord(float x, float y, float R_max);
	double get_random(double begin, double end);
	void update(sf::Time dt);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif