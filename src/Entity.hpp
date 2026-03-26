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

class Entity: public sf::Drawable
{
public:
	enum Type
	{
		ROCK,
		PAPER,
		SCISSORS,
		UNKNOWN,
		ALL
	};
	struct Pair
	{
		Type type;
		sf::FloatRect rect;
	};

private:
	static inline std::vector<Entity::Pair> table{};
	static inline std::array<sf::Texture, Type::ALL> m_texturebuffers;
	static inline std::array<sf::SoundBuffer, Type::ALL> m_soundbuffers;
	static inline int m_id_counter{0};
	
	float m_speed = 80;

	//----------------------------------------------------------------//
	// Do not change order of these fields, because we initialize them 
	// in member initializer list exactly in the order they are declared here
	Type m_type;
	sf::Sprite m_sprite;
	sf::Sound m_sound;
	sf::Vector2f m_direction;
	int m_id;
	//----------------------------------------------------------------//

	sf::Vector2f m_pos;

public:
	Entity(Type type);
	Entity(const Entity& value);
	void update(sf::Time dt);
	void setPos(float x, float y);
	void setPos(sf::Vector2f pos);
	static void loadMedia();

private:
	bool beats(const Type defender);
	bool loses_to(const Type attacker);

	sf::Vector2f get_mate_direction();
	void update_table();
	bool collide(const sf::FloatRect& rect) const;
	void collisions_with_walls(char direct);
	void move(sf::Time dt);
	void check_captured();
	void update_direction();
	sf::Vector2f get_direction(bool hunter_flag);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif