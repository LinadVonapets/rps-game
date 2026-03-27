#include "EntityGroupSystem.hpp"

#include "Core.hpp"

EntityGroupSystem::EntityGroupSystem()
	:random_engine{random_device()}
{
}

void EntityGroupSystem::spawn_group(Entity::Type type, int amount, sf::Vector2f pos, float radius)
{
	if (type == Entity::Type::UNKNOWN)
	{
		std::cerr << "EntityGroupSystem::spawn_group: UNKNOWN type specified!" << std::endl;
		return;
	}
		
	for(int i = 0; i < amount; ++i) {
		Entity temp(type);
		temp.setPos(this->get_randomized_coord(pos.x, pos.y, radius));
		this->entities.push_back(temp);
	}
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

double EntityGroupSystem::get_random(double begin, double end)
{
	std::uniform_real_distribution<> dist(begin, end);
	return dist(random_engine);
}

void EntityGroupSystem::update(sf::Time dt)
{
	if (Core::get_instance().get_user_interface().clear_all_entity)
	{
		this->entities.clear();
		Entity::clear_table();
		Core::get_instance().get_user_interface().clear_all_entity = false;
	}
	for(Entity& entity: this->entities)
		entity.update(dt);
}

void EntityGroupSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Entity& entity: this->entities)
		target.draw(entity, states);
}
