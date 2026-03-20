#include "Entity.hpp"


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
    :m_type{value.m_type}, m_direction{value.m_direction}, m_id{value.m_id}, 
    m_sprite(value.m_sprite), m_sound{value.m_sound}
{
    update_table();
}


void Entity::update()
{
    update_direction();
    check_captured();
    move();
    update_table();
}


void Entity::setPos(float x, float y)
{
    m_sprite.setPosition({x, y});
}

void Entity::setPos(Vec2f pos)
{
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
    if (direct == 'w')
    {
        if (m_pos.x + m_sprite.getGlobalBounds().size.x > g_window_rect.size.x)
            m_pos.x = g_window_rect.size.x - m_sprite.getGlobalBounds().size.x;
        if (m_pos.x < g_window_rect.position.x)
            m_pos.x = g_window_rect.position.x;
    }
    if(direct == 'h')
    {    
        if (m_pos.y + m_sprite.getGlobalBounds().size.y > g_window_rect.size.y)
            m_pos.y = g_window_rect.size.y - m_sprite.getGlobalBounds().size.y;
        if (m_pos.y < g_window_rect.position.y)
            m_pos.y = g_window_rect.position.y;
    }    
}


void Entity::move()
{
    m_pos = m_sprite.getPosition();
    if (m_direction.magnitude() != 0)
        m_direction = m_direction.normalize();
    
    m_pos.x += m_direction.x * m_speed;
    collisions_with_walls('w');
    m_pos.y += m_direction.y * m_speed;
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
            if(m_type == ROCK && enemy.type == PAPER || m_type == PAPER && enemy.type == SCISSORS || m_type == SCISSORS && enemy.type == ROCK)
            {
                m_type = enemy.type;
                m_sound.setBuffer(m_soundbuffers[m_type]);
                m_sprite.setTexture(m_texturebuffers[m_type]);
            }

            if(m_type == ROCK && enemy.type == SCISSORS || m_type == PAPER && enemy.type == ROCK || m_type == SCISSORS && enemy.type == PAPER)
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


Vec2f Entity::get_direction(bool hunter_flag)
{
    std::vector<float> distances;
    std::vector<Vec2f> directions;
    for(const auto& entity: table)
    {
        float distance;
        Vec2f direction;
        Vec2f victim_pos(entity.rect.position.x, entity.rect.position.y);
        distance = (victim_pos - Vec2f(m_sprite.getPosition())).magnitude();
        if(distance > 0)
            direction = (victim_pos - Vec2f(m_sprite.getPosition())).normalize();
        else
            direction = {0,0};
        
        
        if (hunter_flag)
        {
            if (entity.type == ROCK && m_type == SCISSORS || entity.type == PAPER && m_type == ROCK || entity.type == SCISSORS && m_type == PAPER)
            {
                distances.push_back(distance);
                directions.push_back(direction);
            }
        }   
        else
        {
            if(entity.type == ROCK && m_type == PAPER || entity.type == PAPER && m_type == SCISSORS || entity.type == SCISSORS && m_type == ROCK)
            {
                distances.push_back(distance);
                directions.push_back(direction);
            }
        }
    }
    
    if (!distances.empty() && !directions.empty())
    {
        float min_dinstance = *std::min_element(distances.begin(), distances.end());
        std::vector<float>::iterator it = std::find(distances.begin(), distances.end(), min_dinstance);
        int index = std::distance(distances.begin(), it);
        return directions[index];
    }
    return {0,0};
}


void Entity::loadMedia()
{
    if(!m_texturebuffers[Entity::ROCK].loadFromFile("assets/rock.png"))
    {
        std::cout << "Failed to load texture: 'assets/rock.png'\n";
    }

    if(!m_texturebuffers[Entity::PAPER].loadFromFile("assets/paper.png"))
    {
        std::cout << "Failed to load texture: 'assets/paper.png'\n";
    }

    if(!m_texturebuffers[Entity::SCISSORS].loadFromFile("assets/scissors.png"))
    {
        std::cout << "Failed to load texture: 'assets/scissors.png'\n";
    }

    if(!m_soundbuffers[Entity::ROCK].loadFromFile("assets/rock.wav"))
    {
        std::cout << "Failed to load sound: 'assets/rock.wav'\n";
    }

    if(!m_soundbuffers[Entity::PAPER].loadFromFile("assets/paper.wav"))
    {
        std::cout << "Failed to load sound: 'assets/paper.wav'\n";
    }

    if(!m_soundbuffers[Entity::SCISSORS].loadFromFile("assets/scissors.wav"))
    {
        std::cout << "Failed to load sound: 'assets/scissors.wav'\n";
    }
}


void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
