#include "EntityGroupSystem.hpp"


EntityGroupSystem::EntityGroupSystem()
	:m_random_engine{m_dev()}
{
}

void EntityGroupSystem::spawn_group(Entity::Type type, int amount, sf::Vector2f pos, float radius)
{
	for(int i = 0; i < amount; ++i) {
		Entity temp(type);
		temp.setPos(randomize_coord(pos.x, pos.y, radius));
		m_entities.push_back(temp);
	}
}

sf::Vector2f EntityGroupSystem::randomize_coord(float x, float y, float R_max)
{
	double angle = get_random(0.f, std::acos(-1)); // acos(-1) used here to generate PI constant with double precision
	int radius = get_random(0, R_max);
	float x_offseted = std::cos(angle) * radius + x;
	float y_offseted = std::sin(angle) * radius + y;

	return {x_offseted, y_offseted};
}

double EntityGroupSystem::get_random(double begin, double end)
{
	std::uniform_real_distribution<> dist(begin, end);
	return dist(m_random_engine);
}

void EntityGroupSystem::update(sf::Time dt)
{
	for(Entity& entity: m_entities)
		entity.update(dt);
}

void EntityGroupSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Entity& entity: m_entities)
		target.draw(entity, states);
}
