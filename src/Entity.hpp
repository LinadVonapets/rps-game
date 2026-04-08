#ifndef __RPS_ENTITY_HPP
#define __RPS_ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <array>
#include <utility>
#include <functional>
#include <algorithm>

class EntityGroupSystem;

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

private:
	//----------------------------------------------------------------//
	// Do not change order of these fields, because we initialize them
	// in member initializer list exactly in the order they are declared here
	Type m_type;
	sf::Sprite m_sprite;
	sf::Sound m_sound;
	sf::Vector2f m_direction;
	size_t m_id;
	//----------------------------------------------------------------//
	EntityGroupSystem* entity_group_system = nullptr;

	float m_speed = 80;
	sf::Vector2f m_pos;

	// This ones needed as temporary when searching direction to near hunter/victim
	float hunter_min_dist;
	float victim_min_dist;
	sf::Vector2f hunter_min_dist_dir;
	sf::Vector2f victim_min_dist_dir;

	sf::Vector2f hunter_direction;
	sf::Vector2f victim_direction;

public:
	// Нужен ли конструктор перемещения?
	explicit Entity(Type type);
	Entity(const Entity& p_other);
	void update(sf::Time dt);
	void setPos(float x, float y);
	void setPos(sf::Vector2f pos);

private:
	bool beats(const Type defender);
	bool loses_to(const Type attacker);

	void set_id(size_t p_id);
	void set_entity_group_system(EntityGroupSystem* p_entity_group_system);

	bool collide(const Entity& p_other) const;
	void collisions_with_walls();

	void move(sf::Time dt);
	void check_captured(const Entity& p_other);

	void update_direction();
	void do_direction_search(const Entity& p_other);
	void reset_direction_search();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	friend class EntityGroupSystem;
};

#endif
