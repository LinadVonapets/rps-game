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
	m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());

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

	this->collider = p_other.collider;
}

void Entity::update(sf::Time dt)
{
	this->reset_direction_search();

	for (size_t i = 0; i < entity_group_system->get_next_id(); i++)
	{
		auto entity = entity_group_system->get_entity_by_id(i);
		if (!entity.has_value() || m_id == entity->get().m_id)
			continue;

		this->check_captured(entity->get());
		this->do_direction_search(entity->get());

	}
	this->update_direction();
	this->move(dt);
}

void Entity::setPos(float x, float y)
{
	this->setPos({x,y});
}

void Entity::setPos(sf::Vector2f pos)
{
	this->m_pos = pos;
	m_sprite.setPosition(pos);
	collider.set_pos(pos);
}

bool Entity::beats(const Entity::Type defender)
{
	bool ret = false;
	switch(this->m_type)
	{
	case Entity::ROCK:
		ret = (defender == Entity::SCISSORS);
		break;
	case Entity::SCISSORS:
		ret = (defender == Entity::PAPER);
		break;
	case Entity::PAPER:
		ret = (defender == Entity::ROCK);
		break;
	default:
		break;
	}
	return ret;
}

bool Entity::loses_to(const Entity::Type attacker)
{
	bool ret = false;
	switch(this->m_type)
	{
	case Entity::ROCK:
		ret = (attacker == Entity::PAPER);
		break;
	case Entity::PAPER:
		ret = (attacker == Entity::SCISSORS);
		break;
	case Entity::SCISSORS:
		ret = (attacker == Entity::ROCK);
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

void Entity::collisions_with_walls(char direct)
{
	sf::Rect<std::uint32_t> win_rect = Core::get()->get_window_rect();
	if (direct == 'w')
	{
		if (m_pos.x + m_sprite.getGlobalBounds().size.x > win_rect.size.x)
			m_pos.x = win_rect.size.x - m_sprite.getGlobalBounds().size.x;
		if (m_pos.x < win_rect.position.x)
			m_pos.x = win_rect.position.x;
	}

	if(direct == 'h')
	{
		if (m_pos.y + m_sprite.getGlobalBounds().size.y > win_rect.size.y)
			m_pos.y = win_rect.size.y - m_sprite.getGlobalBounds().size.y;
		if (m_pos.y < win_rect.position.y)
			m_pos.y = win_rect.position.y;
	}
}

void Entity::move(sf::Time dt)
{
	m_pos = m_sprite.getPosition();
	if (m_direction.lengthSquared() != 0)
		m_direction = m_direction.normalized();

	m_pos.x += m_direction.x * m_speed * dt.asSeconds();
	collisions_with_walls('w');
	m_pos.y += m_direction.y * m_speed * dt.asSeconds();
	collisions_with_walls('h');
	this->setPos(m_pos);
}

void Entity::check_captured(const Entity& p_other)
{
	if(collider.collide(p_other.collider))
	{
		if(this->loses_to(p_other.m_type))
		{
			m_type = p_other.m_type;
			m_sound.setBuffer(*Core::get()->get_sound(m_type));
			m_sprite.setTexture(*Core::get()->get_texture(m_type));
		}

		if(this->beats(p_other.m_type))
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

	this->m_direction = this->victim_direction - hunter_direction;
}

void Entity::do_direction_search(const Entity& p_other)
{
	float distance;
	sf::Vector2f direction = {0,0};

	sf::Vector2f difference = (p_other.m_pos - m_pos);

	distance = difference.lengthSquared();
	if(distance > 0)
		direction = difference;


	if (this->loses_to(p_other.m_type))
	{
		if (this->hunter_min_dist > distance)
		{
			this->hunter_min_dist = distance;
			this->hunter_min_dist_dir = direction;
		}
	}
	else if(this->beats(p_other.m_type))
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
