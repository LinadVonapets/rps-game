#include "EntityGroupSystem.hpp"


EntityGroupSystem::EntityGroupSystem(int amount_rocks, int amount_papers, int amount_scisissors, 
									 sf::Vector2f rocks_pos, sf::Vector2f papers_pos, sf::Vector2f scissors_pos, 
									 float group_radius)
	:m_random_engine{m_dev()} // initialize random engine
{
	spawn_group(Entity::SCISSORS, amount_scisissors, scissors_pos, group_radius);
	spawn_group(Entity::PAPER, amount_papers, papers_pos, group_radius);
	spawn_group(Entity::ROCK, amount_rocks, rocks_pos, group_radius);
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


void EntityGroupSystem::update()
{
	for(Entity& entity: m_entities)
		entity.update();
}


void EntityGroupSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Entity& entity: m_entities)
		target.draw(entity, states);
}
