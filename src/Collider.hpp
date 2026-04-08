#ifndef __RPS_COLLIDER_HPP
#define __RPS_COLLIDER_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Collider : public sf::Drawable
{
	sf::CircleShape center_dot;
	sf::RectangleShape shape;
	bool show = true;

public:
	Collider();
	bool collide(const Collider& p_collider);
	sf::Vector2f clamp_to_screen(const sf::Vector2f& p_pos);
	void set_size(sf::Vector2f p_size);
	void set_pos(sf::Vector2f p_pos);

	void set_show(bool p_state);
	bool get_show();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif
