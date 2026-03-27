#include "Entity.hpp"
#include "Core.hpp"

Entity::Entity(Type type)
	: m_type{type}
	, m_sprite(m_texturebuffers[type])
	, m_sound(m_soundbuffers[type])
{
	m_id = m_id_counter++;
	m_sprite.setScale(sf::Vector2f(0.2, 0.2));
	table.push_back({m_type, m_sprite.getGlobalBounds()});
}

Entity::Entity(const Entity& value)
	: m_type{value.m_type}
	, m_sprite(value.m_sprite)
	, m_sound{value.m_sound}
	, m_direction{value.m_direction}
	, m_id{value.m_id}
{
	update_table();
}

void Entity::update(sf::Time dt)
{
	update_direction();
	check_captured();
	move(dt);
	update_table();
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
	if (m_direction.length() != 0)
		m_direction = m_direction.normalized();

	m_pos.x += m_direction.x * m_speed * dt.asSeconds();
	collisions_with_walls('w');
	m_pos.y += m_direction.y * m_speed * dt.asSeconds();
	collisions_with_walls('h');
	m_sprite.setPosition(m_pos);
}

void Entity::check_captured()
{
	for(const auto& enemy: table)
	{
		// checking capture
		if(collide(enemy.rect))
		{
			if(this->loses_to(enemy.type))
			{
				m_type = enemy.type;
				m_sound.setBuffer(m_soundbuffers[m_type]);
				m_sprite.setTexture(m_texturebuffers[m_type]);
			}

			if(this->beats(enemy.type))
			{
				m_sound.play();
			}
		}
	}
}

void Entity::update_direction()
{
	m_direction = get_direction(false) - get_direction(true);
}

sf::Vector2f Entity::get_direction(bool hunter_flag)
{
	std::vector<float> distances;
	std::vector<sf::Vector2f> directions;

	for(const auto& entity: Entity::table) {
		float distance;
		sf::Vector2f direction;
		sf::Vector2f victim_pos(entity.rect.position.x, entity.rect.position.y);

		distance = (victim_pos - m_sprite.getPosition()).length();
		if(distance > 0)
			direction = (victim_pos - m_sprite.getPosition()).normalized();
		else
			direction = {0,0};

		if (hunter_flag)
		{
			if (this->loses_to(entity.type))
			{
				distances.push_back(distance);
				directions.push_back(direction);
			}
		}
		else
		{
			if(this->beats(entity.type))
			{
				distances.push_back(distance);
				directions.push_back(direction);
			}
		}
	}

	if (!(distances.empty() || directions.empty()))
	{
		std::vector<float>::iterator it = std::min_element(distances.begin(), distances.end());
		int index = std::distance(distances.begin(), it);
		return directions[index];
	}
	return {0,0};
}

void Entity::loadMedia()
{
	if(!m_texturebuffers[Entity::ROCK].loadFromFile("assets/rock.png")) {
		std::cout << "Failed to load texture: 'assets/rock.png'\n";
	}

	if(!m_texturebuffers[Entity::PAPER].loadFromFile("assets/paper.png")) {
		std::cout << "Failed to load texture: 'assets/paper.png'\n";
	}

	if(!m_texturebuffers[Entity::SCISSORS].loadFromFile("assets/scissors.png")) {
		std::cout << "Failed to load texture: 'assets/scissors.png'\n";
	}

	if(!m_soundbuffers[Entity::ROCK].loadFromFile("assets/rock.wav")) {
		std::cout << "Failed to load sound: 'assets/rock.wav'\n";
	}

	if(!m_soundbuffers[Entity::PAPER].loadFromFile("assets/paper.wav")) {
		std::cout << "Failed to load sound: 'assets/paper.wav'\n";
	}

	if(!m_soundbuffers[Entity::SCISSORS].loadFromFile("assets/scissors.wav")) {
		std::cout << "Failed to load sound: 'assets/scissors.wav'\n";
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

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
