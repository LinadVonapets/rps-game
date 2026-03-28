#include "EntityGroupSystem.hpp"

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
		temp.setPos(this->get_randomized_coord(p_pos.x, p_pos.y, p_radius));
		this->add_entity(std::move(temp));
	}
}

size_t EntityGroupSystem::add_entity(Entity&& p_entity)
{
	size_t new_id = this->get_next_id();
	p_entity.set_id(new_id);
	p_entity.set_entity_group_system(this);
	entities.push_back(p_entity);
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
	// acos(-1) used here to generate PI constant with double precision
	double Pi = std::acos(-1);

	double angle = this->get_random(0.f, 2*Pi);
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
	if (Core::get_instance().get_user_interface().clear_all_entity)
	{
		this->entities.clear();
		Core::get_instance().get_user_interface().clear_all_entity = false;
	}
	for(Entity& entity: this->entities)
		entity.update(p_dt);
}

void EntityGroupSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Entity& entity: this->entities)
		target.draw(entity, states);
}
