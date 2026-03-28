#include "Entity.hpp"
#include "Core.hpp"

Entity::Entity(Type type)
	: m_sprite(m_texturebuffers[type])
	, m_sound(m_soundbuffers[type])
{
	this->m_type = type;
	this->m_id = m_id_counter++;
	this->hunter_min_dist = std::numeric_limits<float>::max();
	this->victim_min_dist = std::numeric_limits<float>::max();

	m_sprite.setScale(sf::Vector2f(0.2, 0.2));
	table.push_back({m_type, m_sprite.getGlobalBounds()});
}

Entity::Entity(const Entity& p_other)
	: m_sprite(p_other.m_sprite)
	, m_sound{p_other.m_sound}
	, m_direction{p_other.m_direction}
	, m_pos{p_other.m_pos}
	, hunter_min_dist_dir{p_other.hunter_min_dist_dir}
	, victim_min_dist_dir{p_other.victim_min_dist_dir}
{
	this->m_type = p_other.m_type;
	this->m_id = p_other.m_id;
	this->m_speed = p_other.m_speed;
	this->hunter_min_dist = p_other.hunter_min_dist;
	this->victim_min_dist = p_other.victim_min_dist;
	this->hunter_direction = p_other.hunter_direction;
	this->victim_direction = p_other.victim_direction;
	update_table();
}

void Entity::update(sf::Time dt)
{
	this->reset_direction_search();
	for (const auto& entity: Entity::table)
	{
		this->check_captured(entity);
		this->do_direction_search(entity);

	}
	this->update_direction();
	this->move(dt);
	this->update_table();
}

void Entity::setPos(float x, float y)
{
	this->m_pos = {x, y};
	m_sprite.setPosition({x, y});
}

void Entity::setPos(sf::Vector2f pos)
{
	this->m_pos = pos;
	m_sprite.setPosition(pos);
}

void Entity::loadMedia()
{
	std::string image_path[Entity::Type::ALL] =
	{
		"assets/images/rock.png",
		"assets/images/paper.png",
		"assets/images/scissors.png",
		"assets/images/lost.png",
	};

	std::string sound_path[Entity::Type::ALL] =
	{
		"assets/sounds/rock.wav",
		"assets/sounds/paper.wav",
		"assets/sounds/scissors.wav",
		"assets/sounds/lost.wav"
	};

	for(int i = 0; i < Entity::Type::ALL; i++)
	{
		if(!m_texturebuffers[Entity::Type(i)].loadFromFile(image_path[i])) {
			std::cerr << "Failed to load texture: " << "'" << image_path[i] << "'" << std::endl;
		}
		if(!m_soundbuffers[Entity::Type(i)].loadFromFile(sound_path[i])) {
			std::cerr << "Failed to load sound: " << "'" << sound_path[i] << "'" << std::endl;
		}
	}
}

void Entity::clear_table()
{
	m_id_counter = 0;
	table.clear();
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

// inefficiant method, need to find another solution
void Entity::update_table()
{
	table[m_id] = {m_type, m_sprite.getGlobalBounds()};
}

bool Entity::collide(const sf::FloatRect& rect) const
{
	return m_sprite.getGlobalBounds().findIntersection(rect).has_value();
}

void Entity::collisions_with_walls(char direct)
{
	sf::Rect<std::uint32_t> win_rect = Core::get_instance().get_window_rect();
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
	m_sprite.setPosition(m_pos);
}

void Entity::check_captured(const Entity::Pair& p_entity_pair)
{
	if(collide(p_entity_pair.rect))
	{
		if(this->loses_to(p_entity_pair.type))
		{
			m_type = p_entity_pair.type;
			m_sound.setBuffer(m_soundbuffers[m_type]);
			m_sprite.setTexture(m_texturebuffers[m_type]);
		}

		if(this->beats(p_entity_pair.type))
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

void Entity::do_direction_search(const Entity::Pair& p_entity_pair)
{
	float distance;
	sf::Vector2f direction = {0,0};

	sf::Vector2f difference = (p_entity_pair.rect.position - m_sprite.getPosition());

	distance = difference.lengthSquared();
	if(distance > 0)
		direction = difference;


	if (this->loses_to(p_entity_pair.type))
	{
		if (this->hunter_min_dist > distance)
		{
			this->hunter_min_dist = distance;
			this->hunter_min_dist_dir = direction;
		}
	}
	else if(this->beats(p_entity_pair.type))
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
}
