#ifndef __RPS_ENTITY_GROUP_SYSTEM_HPP
#define __RPS_ENTITY_GROUP_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "Entity.hpp"

class EntityGroupSystem: public sf::Drawable
{
private:
	using EntityRef = std::reference_wrapper<Entity>;

	std::random_device random_device;
	std::mt19937 random_engine;

	std::vector<Entity> entities;

public:
	EntityGroupSystem();
	void spawn_group(Entity::Type p_type, int p_amount, sf::Vector2f p_pos, float p_radius);

	size_t add_entity(Entity&& p_entity);
	std::optional<EntityRef> get_entity_by_id(size_t p_id);
	size_t get_next_id() const;

	sf::Vector2f get_randomized_coord(float p_x, float p_y, float p_max_radius);
	double get_random(double p_from, double p_to);
	void update(sf::Time p_dt);

	short grid_cell_width = 200;
	std::unordered_map<size_t, std::unordered_set<size_t>> grid;

	size_t calculate_cell_index(const Entity& p_entity);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif
