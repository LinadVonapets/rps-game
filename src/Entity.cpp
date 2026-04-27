#include "Entity.hpp"
#include "Core.hpp"

Entity::Entity(Type type)
	: m_sprite(*Core::get()->get_texture(type))
	, m_sound(*Core::get()->get_sound(type))
{
	this->entity_group_system = nullptr;
	this->m_type = type;
	this->m_id = 0;
	this->hunter_min_dist = std::numeric_limits<float>::max();
	this->victim_min_dist = std::numeric_limits<float>::max();

	m_sprite.setScale(sf::Vector2f(0.2, 0.2));

	collider.set_size(m_sprite.getGlobalBounds().size);
}

Entity::Entity(const Entity& p_other)
	: m_sprite(p_other.m_sprite)
	, m_sound{p_other.m_sound}
	, m_direction{p_other.m_direction}
	, m_pos{p_other.m_pos}
	, hunter_min_dist_dir{p_other.hunter_min_dist_dir}
	, victim_min_dist_dir{p_other.victim_min_dist_dir}
{
	this->entity_group_system = p_other.entity_group_system;
	this->m_type = p_other.m_type;
	this->m_id = p_other.m_id;
	this->m_speed = p_other.m_speed;
	this->hunter_min_dist = p_other.hunter_min_dist;
	this->victim_min_dist = p_other.victim_min_dist;
	this->hunter_direction = p_other.hunter_direction;
	this->victim_direction = p_other.victim_direction;
	this->grid_cell_index = p_other.grid_cell_index;

	this->collider = p_other.collider;
}

void Entity::update(sf::Time dt)
{
	this->reset_direction_search();

	for (const auto& id : entity_group_system->grid[grid_cell_index])
	{
		auto entity = entity_group_system->get_entity_by_id(id);
		if (!entity.has_value() || m_id == entity->get().m_id)
			continue;

		this->check_captured(entity->get());
		this->do_direction_search(entity->get());
	}
	this->update_direction();
	this->move(dt);
}

void Entity::set_position(float p_x, float p_y)
{
	this->set_position({p_x, p_y});
}

void Entity::set_position(sf::Vector2f p_pos)
{
	this->m_pos = p_pos;
	m_sprite.setPosition(this->m_pos);
	collider.set_pos(this->m_pos);
}

sf::Vector2f Entity::get_position() const
{
	return this->m_pos;
}

bool Entity::beats(const Entity& p_defender) const
{
	bool ret = false;
	switch(this->m_type)
	{
	case Entity::ROCK:
		ret = (p_defender.m_type == Entity::SCISSORS);
		break;
	case Entity::SCISSORS:
		ret = (p_defender.m_type == Entity::PAPER);
		break;
	case Entity::PAPER:
		ret = (p_defender.m_type == Entity::ROCK);
		break;
	default:
		break;
	}
	return ret;
}

bool Entity::loses_to(const Entity& p_attacker) const
{
	bool ret = false;
	switch(this->m_type)
	{
	case Entity::ROCK:
		ret = (p_attacker.m_type == Entity::PAPER);
		break;
	case Entity::PAPER:
		ret = (p_attacker.m_type == Entity::SCISSORS);
		break;
	case Entity::SCISSORS:
		ret = (p_attacker.m_type == Entity::ROCK);
		break;
	default:
		break;
	}
	return ret;
}

void Entity::set_id(size_t p_id)
{
	if (this->m_id != p_id)
		this->m_id = p_id;
}

void Entity::set_entity_group_system(EntityGroupSystem* p_entity_group_system)
{
	if(this->entity_group_system != p_entity_group_system)
		this->entity_group_system = p_entity_group_system;
}

void Entity::set_grid_cell_index(size_t p_index)
{
	this->grid_cell_index = p_index;
}

size_t Entity::get_grid_cell_index() const
{
	return this->grid_cell_index;
}

void Entity::move(sf::Time dt)
{
	sf::Vector2f position = this->get_position();

	if (m_direction.lengthSquared() != 0)
		m_direction = m_direction.normalized();

	position.x += m_direction.x * m_speed * dt.asSeconds();
	position.y += m_direction.y * m_speed * dt.asSeconds();

	position = collider.clamp_to_screen(position);
	this->set_position(position);
}

void Entity::check_captured(const Entity& p_other)
{
	if(collider.collide(p_other.collider))
	{
		if(this->loses_to(p_other))
		{
			m_type = p_other.m_type;
			m_sound.setBuffer(*Core::get()->get_sound(m_type));
			m_sprite.setTexture(*Core::get()->get_texture(m_type));
		}

		if(this->beats(p_other))
		{
			m_sound.play();
		}
	}
}

void Entity::update_direction()
{
	this->victim_direction = {0, 0};
	this->hunter_direction = {0, 0};

	if (victim_min_dist_dir.lengthSquared() != 0)
		this->victim_direction = victim_min_dist_dir.normalized();
	if (hunter_min_dist_dir.lengthSquared() != 0)
		this->hunter_direction = hunter_min_dist_dir.normalized();

	this->m_direction = this->victim_direction - this->hunter_direction;
}

void Entity::do_direction_search(const Entity& p_other)
{
	float distance;
	sf::Vector2f direction = {0,0};

	sf::Vector2f difference = (p_other.m_pos - m_pos);

	distance = difference.lengthSquared();
	if(distance > 0)
		direction = difference;


	if (this->loses_to(p_other))
	{
		if (this->hunter_min_dist > distance)
		{
			this->hunter_min_dist = distance;
			this->hunter_min_dist_dir = direction;
		}
	}
	else if(this->beats(p_other))
	{
		if (this->victim_min_dist > distance)
		{
			this->victim_min_dist = distance;
			this->victim_min_dist_dir = direction;
		}
	}
}

void Entity::reset_direction_search()
{
	this->hunter_min_dist = std::numeric_limits<float>::max();
	this->victim_min_dist = this->hunter_min_dist;
	this->hunter_min_dist_dir = {0, 0};
	this->victim_min_dist_dir = {0, 0};
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(collider, states);
}
