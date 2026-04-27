#include "EntityGroupSystem.hpp"

#include <numbers>

#include "Core.hpp"

EntityGroupSystem::EntityGroupSystem()
	:random_engine{random_device()}
{
}

void EntityGroupSystem::spawn_group(Entity::Type p_type, int p_amount, sf::Vector2f p_pos, float p_radius)
{
	if (p_type == Entity::Type::UNKNOWN)
	{
		std::cerr << "EntityGroupSystem::spawn_group: UNKNOWN type specified!" << std::endl;
		return;
	}

	for(int i = 0; i < p_amount; ++i) {
		Entity temp(p_type);
		temp.set_position(this->get_randomized_coord(p_pos.x, p_pos.y, p_radius));
		this->add_entity(std::move(temp));
	}
}

size_t EntityGroupSystem::add_entity(Entity&& p_entity)
{
	size_t new_id = this->get_next_id();
	p_entity.set_id(new_id);
	p_entity.set_entity_group_system(this);
	entities.push_back(p_entity);
	p_entity.set_grid_cell_index(this->calculate_cell_index(p_entity));
	grid[p_entity.get_grid_cell_index()].insert(p_entity.m_id);
	return new_id;
}

std::optional<std::reference_wrapper<Entity>> EntityGroupSystem::get_entity_by_id(size_t p_id)
{
	if (p_id < this->get_next_id())
		return std::make_optional<EntityRef>(std::ref(entities[p_id]));
	return std::nullopt;
}

size_t EntityGroupSystem::get_next_id() const
{
	return entities.size();
}

sf::Vector2f EntityGroupSystem::get_randomized_coord(float p_x, float p_y, float p_max_radius)
{
	double angle = this->get_random(0.f, 2 * std::numbers::pi);
	int radius = this->get_random(0, p_max_radius);

	float x_offseted = std::cos(angle) * radius + p_x;
	float y_offseted = std::sin(angle) * radius + p_y;

	return {x_offseted, y_offseted};
}

double EntityGroupSystem::get_random(double p_from, double p_to)
{
	std::uniform_real_distribution<> dist(p_from, p_to);
	return dist(random_engine);
}

void EntityGroupSystem::update(sf::Time p_dt)
{
	if (Core::get()->get_user_interface().clear_all_entity)
	{
		this->entities.clear();
		Core::get()->get_user_interface().clear_all_entity = false;
	}
	for(Entity& entity: this->entities)
	{
		size_t new_index = this->calculate_cell_index(entity);
		size_t current_index = entity.get_grid_cell_index();

		if (new_index != current_index)
		{
			grid[current_index].erase(entity.m_id);
			entity.set_grid_cell_index(new_index);
			grid[new_index].insert(entity.m_id);
		}
		entity.update(p_dt);
	}
}

size_t EntityGroupSystem::calculate_cell_index(const Entity& p_entity)
{
	sf::Vector2i cell_coordinates = sf::Vector2i(p_entity.get_position()) / static_cast<int>(this->grid_cell_width);
	return (static_cast<size_t>(cell_coordinates.x) << 32) | static_cast<size_t>(cell_coordinates.y);
}

void EntityGroupSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Entity& entity: this->entities)
		target.draw(entity, states);
}
