#ifndef __RPS_ENTITY_HPP
#define __RPS_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <algorithm>

extern const sf::FloatRect g_window_rect;


class Entity: public sf::Drawable
{
public:
    enum Type
    {
        ROCK,
        PAPER,
        SCISSORS,
        ALL
    };
    struct Pair
    {
        Type type;
        sf::FloatRect rect;
    };

private:    
    float m_speed = 0.5;
    static inline std::vector<Entity::Pair> table{};
    static inline int m_id_counter{0};

    static inline std::array<sf::Texture, Type::ALL> m_texturebuffers;
    sf::Sprite m_sprite;
    
    static inline std::array<sf::SoundBuffer, Type::ALL> m_soundbuffers;
    sf::Sound m_sound;

    int m_id;

    sf::Vector2f m_direction;
    Type m_type;
    sf::Vector2f m_pos;
    
public:
    Entity(Type type);
    Entity(const Entity& value);
    void update();
    void setPos(float x, float y);
    void setPos(sf::Vector2f pos);  
    static void loadMedia();
    
private:
    sf::Vector2f get_mate_direction();
    void update_table();
    bool collide(const sf::FloatRect& rect) const;
    void collisions_with_walls(char direct);    
    void move();
    void check_captured();
    void update_direction();
    sf::Vector2f get_direction(bool hunter_flag);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif