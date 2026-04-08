#include "Collider.hpp"

#include "Core.hpp"

Collider::Collider()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	center_dot.setRadius(1.f);
	center_dot.setFillColor(sf::Color::Red);
}

bool Collider::collide(const Collider& p_collider)
{
	sf::FloatRect our_rect = this->shape.getGlobalBounds();
	sf::FloatRect other_rect = p_collider.shape.getGlobalBounds();
	return our_rect.findIntersection(other_rect).has_value();
}

sf::Vector2f Collider::clamp_to_screen(const sf::Vector2f& p_pos)
{
	sf::Rect<std::uint32_t> screen = Core::get()->get_window_rect();

	float screen_left = screen.position.x;
	float screen_right = screen.size.x - shape.getGlobalBounds().size.x;

	float screen_top = screen.position.y;
	float screen_bottom = screen.size.y - shape.getGlobalBounds().size.y;

	sf::Vector2f position = sf::Vector2f(
		std::clamp(p_pos.x, screen_left, screen_right),
		std::clamp(p_pos.y, screen_top, screen_bottom)
	);

	return position;
}

void Collider::set_size(sf::Vector2f p_size)
{
	shape.setSize(p_size);
}

void Collider::set_pos(sf::Vector2f p_pos)
{
	// We need set pos for shape even if it not shown
	// because we use this shape to detect collisions
	shape.setPosition(p_pos);

	if(this->show)
		center_dot.setPosition(p_pos + shape.getLocalBounds().getCenter());
}

void Collider::set_show(bool p_state)
{
	this->show = p_state;
}

bool Collider::get_show()
{
	return this->show;
}

void Collider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!this->show)
		return;

	target.draw(center_dot);
	target.draw(shape, states);
}
