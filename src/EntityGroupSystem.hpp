#ifndef __RPS_ENTITY_GROUP_SYSTEM_HPP
#define __RPS_ENTITY_GROUP_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <random>

#include "Entity.hpp"

class EntityGroupSystem: public sf::Drawable
{
private:
	std::random_device random_device;
	std::mt19937 random_engine;

	std::vector<Entity> entities;


public:
	EntityGroupSystem();
	void spawn_group(Entity::Type type, int amount, sf::Vector2f pos, float radius);
	sf::Vector2f get_randomized_coord(float p_x, float p_y, float p_max_radius);
	double get_random(double begin, double end);
	void update(sf::Time dt);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif
